#ifndef _THREAD_H_
#define _THREAD_H_

// #include "current_thread.h"
#include <pthread.h>
#include <functional>
#include <memory>
#include "CountDownLatch.h"
#include <string>

class Thread {
public:

    typedef std::function<void()> ThreadFunc;

    explicit Thread(const ThreadFunc&, const std::string name );
    ~Thread();
    void start();
    int join();
    bool started();

private:
    ThreadFunc func_;
    std::string name_;
    bool started_;
    bool joined_;
    pthread_t thread_;
    pid_t thread_id;
    CountDownLatch latch_;

};

struct ThreadData{
    typedef Thread::ThreadFunc ThreadFunc;

    ThreadData(const ThreadFunc& func, const std::string& _name, pid_t* _thread_id, CountDownLatch* _latch)
        :func_(func),
         name_(_name),
         thread_id(_thread_id),
         latch_(_latch) {}
    ThreadFunc func_;
    std::string name_;
    pid_t* thread_id;
    CountDownLatch* latch_;
};

#endif
