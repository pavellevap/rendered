//
// Created by pavel on 13.04.16.
//

#ifndef RENDERER_THREADPOOL_H
#define RENDERER_THREADPOOL_H


#include <future>
#include <vector>
#include <stdexcept>
#include "ThreadSafeQueue.h"

template<class T>
class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency()) : isClosed(false) {
        if (numThreads == 0)
            numThreads = 1;

        for(size_t i = 0; i < numThreads; i++)
            threads.emplace_back([this] () {
                std::packaged_task<T()> func;
                while(queue.dequeue(func))
                    func();
            });
    }

    std::future<T> submit(std::function<T()> func)  {
        if (isClosed)
            throw std::logic_error("Submitting in closed pool");
        std::packaged_task<T()> task(func);
        std::future<T> result = task.get_future();
        queue.enqueue(std::move(task));
        return result;
    }

    void shutdown()  {
        isClosed = true;
        queue.shutdown();
        joinThreads();
    }

    ~ThreadPool()  {
        shutdown();
    }
private:
    bool isClosed;
    ThreadSafeQueue< std::packaged_task<T()> > queue;
    std::vector<std::thread> threads;

    void joinThreads() {
        for (size_t i = 0; i < threads.size(); i++) {
            if (threads[i].joinable())
                threads[i].join();
        }
    }
};


#endif //RENDERER_THREADPOOL_H
