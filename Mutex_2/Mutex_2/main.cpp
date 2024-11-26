#define _USE_MATH_DEFINES
#include<iostream>
#include<thread>
#include<mutex>
#include<vector>

using namespace std;
mutex m1;
mutex m2;

void deadlock_func_1()
{
    cout << "Start deadlock_func_1..." << endl;
    scoped_lock L{ m1, m2 };
    //lock(m1, m2);
    //lock_guard<mutex> la{m1, adopt_lock};
    //unique_lock<mutex>la{ m1, defer_lock };
    this_thread::sleep_for(100ms);
    cout << "deadlock_func_1 acquires mutex 1..." << endl;
    //lock_guard<mutex> lb{m2, adopt_lock};
    //unique_lock<mutex>lb{ m2, defer_lock };
    //lock(m1, m2);
    cout << "Never print A" << endl;
}

void deadlock_func_2()
{
    scoped_lock L{ m1, m2 };
    //lock(m1, m2);
    cout << "Start deadlock_func_2..." << endl;
    //lock_guard<mutex> lb{m2, adopt_lock};
    //unique_lock<mutex>lb{ m2, defer_lock };
    this_thread::sleep_for(200ms);
    cout << "deadlock_func_2 acquires mutex 2..." << endl;
    //lock_guard<mutex> la{m1, adopt_lock};
    //unique_lock<mutex>la{ m1, defer_lock };
    cout << "Never print B" << endl;
    //lock(m1, m2);
}

int main_t2()
{
    thread t1(deadlock_func_1);
    thread t2(deadlock_func_2);
    t1.join();
    t2.join();


    system("Pause");
    return 0;
}
