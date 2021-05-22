#include "EventLoopThread.hpp"
#include "logger.h"
#include <sys/syscall.h>

EventLoopThread::EventLoopThread(const thread_callback& cb, const std::string& name) 
    : loop_(nullptr)
    , thread_(std::bind(&EventLoopThread::thread_func, this), name) 
    , mutex_()
    , cond_(mutex_)
    , callback_(cb) {}

EventLoopThread::~EventLoopThread() {
    if (loop_ != nullptr) {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::start_loop() {
    // QUESTION:在这里start不会出现问题？
    // LOG<<"start_loop--->3";
    // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->4";
    thread_.start();
    EventLoop* real_loop = nullptr;
    {
        MutexLockGuard lock(mutex_);
        while(loop_ == nullptr) {
            // it won't run until thread running in thread_func in line cond_notify()
            cond_.wait();
        }
        real_loop = loop_;
    }
    return real_loop;
}

void EventLoopThread::thread_func() {
    // LOG<<"thread_func--->4";
    EventLoop loop;
    // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->3";
    if (callback_) {
        callback_(&loop);
    }
    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        cond_.notify();
    }
    loop.loop();
    MutexLockGuard lock(mutex_);
    loop_ = nullptr;
}