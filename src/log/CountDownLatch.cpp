#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int cnt) 
        :count(cnt),
         mutex_(),
         condition_(mutex_){}


void CountDownLatch::wait() {
    MutexLockGuard lock(mutex_);
    while (count > 0) condition_.wait();
}


void CountDownLatch::count_down() {
    MutexLockGuard lock(mutex_);
    --count;
    if (count == 0) condition_.notify_all();
}