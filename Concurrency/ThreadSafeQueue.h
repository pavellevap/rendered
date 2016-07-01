//
// Created by pavel on 13.04.16.
//

#ifndef RENDERER_THREADSAFEQUEUE_H
#define RENDERER_THREADSAFEQUEUE_H


#include <deque>
#include <cstddef>
#include <condition_variable>

template <class Value, class Container = std::deque<Value>>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() : isClosed(false) {}

    void enqueue(Value item)  {
        std::unique_lock<std::mutex> Lock(lock);

        if (isClosed)
            throw "Pushing in closed queue\n";

        queue.emplace_back(std::move(item));

        isEmpty.notify_one();
    }

    // true if extraction is correct (queue wasn't empty or shutdown wasn't called
    bool dequeue(Value &item)  {
        std::unique_lock<std::mutex> Lock(lock);
        isEmpty.wait(Lock, [this] () { return !queue.empty() || isClosed; });

        if (isClosed && queue.empty())
            return false;

        item = std::move(queue.front());
        queue.pop_front();

        return true;
    }

    void shutdown()  {
        isClosed = true;
        isEmpty.notify_all();
    }

private:
    bool isClosed;

    Container queue;

    std::condition_variable isEmpty;

    std::mutex lock;
};

#endif //RENDERER_THREADSAFEQUEUE_H
