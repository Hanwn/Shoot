#include "Thread.h"
#include <assert.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/unistd.h>
#include <linux/prctl.h>
#include <linux/unistd.h>
#include "Thread.h"

void* start_thread(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);

    // awake up main thread start function
    data->latch_->count_down();

    // thread_name_ = data->name_.empty() ? "Thread" : data->name_.c_str();
    // BUG:how to use prctl function
    // prctl(PR_SET_NAME , thread_name_);
    
    data->func_();
    // thread_status_ = "finished";
    delete data;
    // FIXME: pthread_exit();? would be better?
    return nullptr;
}


Thread::Thread(const ThreadFunc& func, const std::string name)
    :func_(func),
     name_(name),
     started_(false),
     joined_(false),
     thread_id(0),
     thread_(0) ,
     latch_(1) {}


Thread::~Thread() {
    // 线程分离
    if (started_ && !joined_) pthread_detach(thread_);
}


void Thread::start() {
    started_ = true;
    ThreadData* data = new ThreadData(func_, name_, &thread_id, &latch_);
    int status = pthread_create(&thread_, nullptr, start_thread, data);
    if (status) {
        started_ = false;
        delete data;
    } else {
        // main thread will wait here until sub thread run to "wake up main start function"
        latch_.wait();
    }
}


int Thread::join() {
    joined_ = true;
    return pthread_join(thread_, nullptr);
}