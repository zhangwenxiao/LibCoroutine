#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include "Coroutine.h"
#include <memory>
#include <vector>
#include <ucontext.h>


namespace corou {

class Schedule {
public:
    Schedule();
    ~Schedule();

    int addCoroutine(const CoroutineFunc& func);
    void resume(int id);
    void yield();

    Status getStatus(int id) const;
    int getRunningId() const { return running_; }
    void setRunningId(int id) { running_ = id; }

    std::shared_ptr<Coroutine>& getCoroutine(int id) 
    { return coroutines_[id]; }

    int& getNumCoroutine() { return numCoroutine_; }

private:
    static void mainFunc(uint32_t low32, uint32_t high32);

private:
    char stack_[STACK_SIZE];
    ucontext_t main_;
    int numCoroutine_;
    int running_;
    std::vector<std::shared_ptr<Coroutine>> coroutines_;
};

}; // namespace corou

#endif
