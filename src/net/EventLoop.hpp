#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include "Channel.hpp"
#include "Epoll.hpp"
#include "mutex_lock.h"
#include <memory>
#include <vector>
// EventLoop 拥有一个文件描述符，需要在析构函数中释放这个文件描述符
class Channel;
class Epoll;
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

    bool is_in_cur_loop();

    void run_in_loop(callback&& cb);
    void queue_in_loop(callback&& );
    void do_pending_functions();
    void wake_up_cur_thread();
    void wake_up_response();
    void quit();
private:
    void handle_expired_time();

private:
    std::unique_ptr<Epoll> poll_;
    std::vector<Channel*> active_vector;
    mutable MutexLock mutex_;
    std::vector<callback> pending_functions_;
    int cur_thread_id_;
    bool handle_pending_functions;
    int wake_up_fd_;
    std::shared_ptr<Channel> wake_up_channel_;
    bool quit_;
};


#endif