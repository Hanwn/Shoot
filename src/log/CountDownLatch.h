#ifndef COUNTDOWNLATCH_H_
#define COUNTDOWNLATCH_H_
#include "Condition_.h"
#include "mutex_lock.h"

class CountDownLatch {
public:
    explicit CountDownLatch(int cnt);
    CountDownLatch(const CountDownLatch& para) = delete;
    ~CountDownLatch() = default;
    const CountDownLatch& operator=(const CountDownLatch& para) = delete;

public:
    void wait();
    void count_down();

private:
    int count;
    mutable MutexLock mutex_;
    Condition condition_;
};
#endif