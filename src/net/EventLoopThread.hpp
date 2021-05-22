#ifndef EVENTLOOPTHREAD_H_
#define EVENTLOOPTHREAD_H_

#include <functional>
#include "EventLoop.hpp"
#include "Thread.h"
#include "Condition_.h"


class EventLoopThread{
public:

    using thread_callback = std::function<void(EventLoop*)>;

    EventLoopThread(const thread_callback& cb, const std::string& name);
    ~EventLoopThread();

public:
    EventLoop* start_loop();

private:
    void thread_func();
    EventLoop* loop_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    thread_callback callback_;

};


#endif