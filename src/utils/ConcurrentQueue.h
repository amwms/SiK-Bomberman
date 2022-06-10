#ifndef SIK_BOMBERMAN_CONCURRENTQUEUE_H
#define SIK_BOMBERMAN_CONCURRENTQUEUE_H

#include <queue>
#include <mutex>
#include <shared_mutex>

template <class T>
class ConcurrentQueue {
    std::queue<T> queue;
    std::shared_mutex mutex;

public:
    ConcurrentQueue() : queue(), mutex() {}

    void push(T element) {
        std::unique_lock lock{mutex};
        queue.push(element);
    }

    T pop() {
        std::unique_lock lock{mutex};
        T result = queue.front();
        queue.pop();

        return result;
    }

    size_t get_size() {
        std::shared_lock lock{mutex};
        return queue.size();
    }

    bool is_empty() {
        std::shared_lock lock{mutex};
        return queue.empty();
    }

    void lock_queue() {
         mutex.lock();
    }

    void unlock_queue() {
        mutex.unlock();
    }

    std::queue<T> &get_queue_no_mutex() {
        return queue;
    }
};


#endif //SIK_BOMBERMAN_CONCURRENTQUEUE_H
