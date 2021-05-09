#ifndef EVENTLOOPPOOL_H_
#define EVENTLOOPPOOL_H_

#include "EventLoop.hpp"
#include "EventLoopThread.hpp"
#include <iostream>
#include <vector>
#include <memory>


class EventLoopPool{
public:
    using thread_callback = std::function<void(EventLoop*)>;
    EventLoopPool(EventLoop* _base_loop, const std::string& _name);
    ~EventLoopPool() = default;

public:
    EventLoop* get_next_loop();
    void start(const thread_callback cb);
    void set_thread_nums(int _thread_num);

private:
    EventLoop* base_loop_;
    std::string name_;
    bool started_;
    int thread_num_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};

#endif