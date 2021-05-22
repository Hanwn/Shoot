#ifndef BLOCKING_QUEUE_H_
#define BLOCKING_QUEUE_H_

#include "mutex_lock.h"
#include <queue>
#include "Condition_.h"

template<typename T>
class BlockingQueue{
public:
    BlockingQueue()
        : mutex_(),
          cond_(mutex_),
          queue_() {}
    ~BlockingQueue() = default;

public:
    void put(T& v) {
        MutexLockGuard(mutex_);
        queue_.push_back(v);
        cond_.notify();

    }

    void put(T&& v) {
        MutexLockGuard(mutex_);
        queue_.push_back(std::move(v));
        cond_.notify();
    }

    T get() {
        MutexLockGuard(mutex_);
        while (queue_.empty()) {
            cond_.wait();
        }
        // 使用移动构造函数，避免使用拷贝构造函数
        T front = std::move(queue_.front());
        queue_.pop_back();
        return front;
    }

    size_t size() {
        MutexLockGuard(mutex_);
        return queue_.size();
    }

private:

    mutable MutexLock mutex_;
    Condition cond_;
    std::deque<T> queue_;

};


#endif