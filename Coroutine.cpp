#include "Coroutine.h"

using namespace corou;

Coroutine::Coroutine(Schedule* sch, CoroutineFunc func)
    : func_(func),
      sch_(sch),
      cap_(0),
      size_(0),
      status_(COROUTINE_READY),
      stack_(nullptr)
{
    ::bzero(&ctx_, sizeof(ctx_));
}

Coroutine::~Coroutine()
{
    if(stack_)
        delete stack_;
}

void Coroutine::saveStack(char* top)
{
    char dummy = 0;
    assert(top - &dummy <= STACK_SIZE);
    if(cap_ < top - &dummy) {
        delete stack_;
        cap_ = top - &dummy;
        stack_ = new char[cap_];
    }
    size_ = top - &dummy;
    memcpy(stack_, &dummy, size_);
}

void Coroutine::run(Schedule* sch)
{
    func_(sch);
}

