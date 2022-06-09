#ifndef SIK_BOMBERMAN_CONCURRENTQUEUE_H
#define SIK_BOMBERMAN_CONCURRENTQUEUE_H

#include <queue>
#include <mutex>

template <class T>
class ConcurrentQueue {
    std::queue<T> queue;
    std::mutex mutex;

public:
    ConcurrentQueue() : queue(), mutex() {}

    void push(T element) {
        mutex.lock();
        queue.push(element);
        mutex.unlock();
    }

    T pop() {
        T result;

        mutex.lock();
        result = queue.pop();
        mutex.unlock();

        return result;
    }
};


#endif //SIK_BOMBERMAN_CONCURRENTQUEUE_H
