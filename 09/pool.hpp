#pragma once
#include <vector>
#include <ostream>
#include <istream>
#include <iostream>
#include <ctype.h>
#include <stdexcept>
#include <functional>
#include <unistd.h>


class ThreadPool
{
    std::atomic<bool> time_to_die {false};
    std::mutex qmutex;
    std::condition_variable new_task_or_end;
    std::deque<std::function<void()>> deq;
    std::vector<std::thread> threads;
public:
    explicit ThreadPool(size_t poolSize)
    {
        for(size_t i = 0; i < poolSize; ++i)
        {
            threads.emplace_back(&ThreadPool::worker, this);
        }
    }

    ~ThreadPool()
    {
        time_to_die = true;
        new_task_or_end.notify_all();
        for(auto& thread : threads)
            thread.join();
    }
    void worker()
    {
        while(true)
        {

            std::unique_lock<std::mutex> lock(qmutex);
            while(!deq.size() && !time_to_die)
                new_task_or_end.wait(lock);
            if(time_to_die)
                return;
            auto task = deq.front();
            deq.pop_front();
            lock.unlock();
            task();
        }
    }

    //pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {
        auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>(std::bind(func, args...));
        auto future = task->get_future();
        std::unique_lock<std::mutex> lock(qmutex);
        deq.emplace_back([task](){(*task)();});
        lock.unlock();
        new_task_or_end.notify_one();
        return future;
    }


};

