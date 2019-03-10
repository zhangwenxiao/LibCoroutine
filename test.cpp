#include "Schedule.h"
#include <iostream>

using namespace corou;
using namespace std;

void foo(Schedule* s)
{
    for(int i = 0; i < 5; ++i) {
        cout << "coroutine " << s -> getRunningId() << " : " << i << endl;
        s -> yield();
    }
}

void test(Schedule& s)
{
    int co1 = s.addCoroutine(foo);
    int co2 = s.addCoroutine(foo);
    cout << "main start" << endl;
    while(s.getStatus(co1) != COROUTINE_DEAD && s.getStatus(co2) != COROUTINE_DEAD) {
        s.resume(co1);
        s.resume(co2);
    }
    cout << "main end" << endl;
}

int main()
{
    Schedule s;
    test(s);
    return 0;
}
