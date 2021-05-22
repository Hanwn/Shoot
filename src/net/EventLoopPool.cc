#include "EventLoopPool.hpp"
#include "logger.h"
#include <sys/syscall.h>
#include <unistd.h>


EventLoopPool::EventLoopPool (EventLoop* _base_loop, const std::string& name)
    : base_loop_(_base_loop)
    , name_(name)
    , started_(false)
    , thread_num_(0)
    , next_(0){

}

void EventLoopPool::start(const thread_callback cb) {
    // assert_in_loop;
    // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->2";
    started_ = true;
    for (int i = 0; i < thread_num_; ++i) {
        char buf[name_.size() + 32];
        snprintf(buf, sizeof buf, "%s%d", name_.c_str(), i);
        EventLoopThread* t = new EventLoopThread(cb, buf);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        loops_.push_back(t->start_loop());
    }
    if (thread_num_ == 0 && cb) {
        cb(base_loop_);
    }
}

EventLoop* EventLoopPool::get_next_loop() {
    // assert_in_loop
    EventLoop* loop = base_loop_;
    if (!loops_.empty()) {
        loop = loops_[next_];
        next_ = (next_ + 1)%loops_.size();

    }
    return loop;
}

void EventLoopPool::set_thread_nums(int _thread_num) {
    thread_num_ = _thread_num;
}