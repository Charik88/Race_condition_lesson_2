#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <queue>
#include <iostream>
using namespace std;

bool flag = false;
mutex m3;

void wait_flag()
{
    unique_lock<mutex> l(m3);
    while (!flag)
    {
        l.unlock();
        this_thread::sleep_for(200ms);
        l.lock();
        cout << "Flag not set" << endl;
    }
    cout << "Flag was set!!!" << endl;
}

void set_flag()
{
    this_thread::sleep_for(1s);
    unique_lock<mutex> l(m3);
    flag = true;
    l.unlock();
}

mutex mut;
queue<int> data_queue;
condition_variable data_cond;

void data_preparation_thread()
{
    int cnt = 0;
    while (cnt < 10)
    {
        int const data = rand() % 100;
        std::lock_guard<std::mutex> lk(mut);
        this_thread::sleep_for(10ms);
        data_queue.push(data);
        cout << "Push " << data << endl;
        data_cond.notify_all();
        if (data == 0)
            break;
    }
}

void data_processing_thread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [] {return !data_queue.empty(); });
        int  data = data_queue.front();
        data_queue.pop();
        lk.unlock();
        cout <<"  Pop " << data << endl;
        if (data == 0)
            break;
    }
}




int main_t3()
{

    //thread t3(wait_flag);
    //thread t4(set_flag);
    //t3.join();
    //t4.join();

    thread t1(data_preparation_thread);
    thread t2(data_processing_thread);
    t1.join();
    t2.join();
    return 0;
}
