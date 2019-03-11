#include "Schedule.h"
#include <iostream>

using namespace std;
using namespace corou;

void foo(Schedule* s, int n)
{
    for(int i = 0; i < n; ++i)
        s -> yield();
}

void test(Schedule& s)
{
    int n = 100000;
    int co1 = s.addCoroutine([n](Schedule* s){ foo(s, n);});
    int co2 = s.addCoroutine([n](Schedule* s){ foo(s, n);});
    cout << "switch times test start!" << endl;
    clock_t start = clock();
    while(s.getStatus(co1) != COROUTINE_DEAD && 
          s.getStatus(co2) != COROUTINE_DEAD) {
        s.resume(co1);
        s.resume(co2);
    }
    clock_t end = clock();
    double switchTimes = n * 2 / ((double)(end - start) / CLOCKS_PER_SEC);
    cout << "switch times test end!" << endl;
    cout << "It can switch " << switchTimes << " times per second" << endl;
}

int main()
{
    Schedule s;
    test(s);

    return 0;
}

