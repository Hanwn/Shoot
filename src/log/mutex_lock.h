#ifndef MUTEX_LOCK_H_
#define MUTEX_LOCK_H_

#include <pthread.h>

class MutexLock{
public:
    MutexLock() {pthread_mutex_init(&mutex, nullptr);}
    ~MutexLock() {
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }
    MutexLock(const MutexLock& mutexlock) = delete;
    const MutexLock& operator=(const MutexLock& mutexlock) = delete;

public:
    void lock() { pthread_mutex_lock(&mutex); }
    void unlock() { pthread_mutex_unlock(&mutex); }
    pthread_mutex_t* get() { return &mutex; }

private:
    pthread_mutex_t mutex;
};


class MutexLockGuard{
public:
    MutexLockGuard(MutexLock& _mutex) :mutex_(_mutex) { mutex_.lock(); }
    ~MutexLockGuard() { mutex_.unlock(); }
private:
    MutexLock &mutex_;
};

#endif