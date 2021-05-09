#include "EventLoopThread.hpp"


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
    EventLoop loop;
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