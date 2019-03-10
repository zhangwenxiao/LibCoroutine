#include "Schedule.h"
#include <cassert>
#include <string.h>

using namespace corou;

Schedule::Schedule()
    : numCoroutine_(0),
      running_(-1)
{
}

Schedule::~Schedule()
{
    coroutines_.clear();
    running_ = -1;
    numCoroutine_ = 0;
}

int Schedule::addCoroutine(const CoroutineFunc& func)
{
    std::shared_ptr<Coroutine> co(new Coroutine(this, func));
    coroutines_.push_back(co);
    ++numCoroutine_;
    return coroutines_.size() - 1;
}

void Schedule::resume(int id)
{
    assert(0 <= id && id < coroutines_.size());

    auto co = coroutines_[id];

    Status status = co -> getStatus();
    ucontext_t& context = co -> getContext();
    switch(status) {
        case COROUTINE_READY:
            getcontext(&context);
            context.uc_stack.ss_sp = stack_;
            context.uc_stack.ss_size = STACK_SIZE;
            context.uc_link = &main_;
            running_ = id;
            co -> setStatus(COROUTINE_RUNNING);
            uintptr_t ptr = (uintptr_t)this;
            makecontext(&context, (void(*)(void))mainFunc, 2, (uint32_t)ptr, (uint32_t)(ptr >> 32));
            swapcontext(&main_, &context);
            break;
        case COROUTINE_SUSPEND:
            ::memcpy(stack_ + STACK_SIZE - co -> size(), co -> stack(), co -> size());
            running_ = id;
            swapcontext(&main_, &context);
            break;
        default:
            assert(false);
    }
}

void Schedule::yield()
{
    assert(running_ >= 0);
    auto co = coroutines_[running_];
    co -> saveStack(stack_ + STACK_SIZE);
    co -> setStatus(COROUTINE_SUSPEND);
    running_ = -1;
    swapcontext(&(co -> getContext()), &main_);
}

Status Schedule::getStatus(int id) const
{
    assert(0 <= id && id < coroutines_.size());
    return coroutines_[id] -> getStatus();
}

void Schedule::mainFunc(uint32_t low32, uint32_t high32)
{
    uintptr_t sptr = (uintptr_t)low32 | ((uintptr_t)high32 << 32);
    Schedule* sch = (Schedule*)sptr;
    int id = sch -> getRunningId();
    std::shared_ptr<Coroutine>& co = sch -> getCoroutine(id);
    co -> run(sch);
    co -> setStatus(COROUTINE_DEAD);
    sch -> setRunningId(-1);
    int& num = sch -> getNumCoroutine();
    --num;
}

