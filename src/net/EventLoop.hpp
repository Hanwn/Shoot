#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include "Channel.hpp"
#include "Epoll.hpp"
#include "mutex_lock.h"

class EventLoop{
public:
    using callback = std::function<void()>;
    EventLoop();
    ~EventLoop();

public:

    void update_channel(Channel* channel);
    void remove_channel(Channel* channel);

public:
    // STAR: this function is readlly Important
    void loop();

    //TODO:2021年4月30日将要实现的函数
    void run_in_curr_loop();
    void queue_in_loop();
    void do_pending_functions();
    void wake_up_cur_thread();

private:
    std::unique_ptr<Epoll> poll_;
    std::vector<Channel*> active_vector;
    mutable MutexLock mutex_;
    std::vector<callback> pending_functions_;
};


#endif