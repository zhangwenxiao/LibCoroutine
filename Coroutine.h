#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <functional>
#include <ucontext.h>

#define STACK_SIZE (1024 * 1024)

namespace corou {

class Schedule;

enum Status {
    COROUTINE_DEAD,
    COROUTINE_READY,
    COROUTINE_RUNNING,
    COROUTINE_SUSPEND
}; // enum Status

using CoroutineFunc = std::function<void(Schedule*)>;

class Coroutine {
public:
    Coroutine(Schedule* s, CoroutineFunc func);
    ~Coroutine();

    Status getStatus() const { return status_; }
    void setStatus(Status status) { status_ = status; }

    char* stack() { return stack_; }
    void saveStack(char* top);

    ptrdiff_t size() const { return size_; }

    void run(Schedule* sch);

    ucontext_t& getContext() { return ctx_; }

private:
    CoroutineFunc func_;
    ucontext_t ctx_;
    Schedule* sch_;
    ptrdiff_t cap_;
    ptrdiff_t size_;
    Status status_;
    char* stack_;
}; // class Coroutine

} // namespace corou

#endif
