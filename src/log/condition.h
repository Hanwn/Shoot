#ifndef CONDITION_H_
#define CONDITION_H_

#include <condition.h>
#include <pthread.h>
#include "mutex_lock.h"
#include <time.h>

class Condition{
public:
    explicit Condition(MutexLock& mutex_) :mutex_(mutex_){ pthread_cond_init(&cond, nullptr); }
    Condition(const Condition& condition) = delete;
    ~Condition() {}
    const Condition& operator=(const Condition& condition) = delete;

public:
    void wait() { pthread_cond_wait(&cond, mutex_);}
    void notify() { pthread_cond_signal(&cond); }
    void notify_all() { pthread_cond_broadcast(&cond); }
    bool wait_seconds(int seconds) {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex_.get(), &abstime);
    }

private:
    MutexLock& mutex_;
    pthread_cond_t cond; 
};

#endif