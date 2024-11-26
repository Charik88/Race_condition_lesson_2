#define _USE_MATH_DEFINES
#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include "Timer.h"

using namespace std;

class spinlock_mutex
{
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    spinlock_mutex() = default;
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire));
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
} sm1, sm2;

void func5()
{
    lock_guard<spinlock_mutex> lg(sm1);
    for (int i = 0; i < 10; i++)
    {
        this_thread::sleep_for(100ms);
        consol_color::SetColor(15, 0);
        cout << i << " ";
    }
    cout << endl;
}
void func6()
{
    lock_guard<spinlock_mutex> grd(sm1);
    for (int i = 0; i < 10; i++)
    {
        this_thread::sleep_for(100ms);
        consol_color::SetColor(10, 0);
        cout << i << " ";
    }
    cout << endl;
    consol_color::SetColor(15, 0);
}


int main()
{
    atomic<int> A;
    cout << boolalpha << A.is_lock_free() << endl;
    //thread t1(func5);
    //thread t2(func6);
    //t1.join();
    //t2.join();
    
    atomic<bool> V1;
    atomic<bool> V2;
    V1 = false;
    V1.store(true);
    cout << boolalpha << V1.load() << endl;
    V1.exchange(false, memory_order_acquire);
    cout << boolalpha << V1.load() << endl;

    A.fetch_add(2);
    atomic<int*> Ptr(new int[10]);
    cout << Ptr << endl;
    Ptr.fetch_add(1);
    cout << Ptr << endl;
    A = 10;
    int k = 9;
    A.compare_exchange_weak(k, 100);
    cout << A << endl;
    cout << k << endl;

    return 0;
}
