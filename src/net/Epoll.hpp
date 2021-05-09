#ifndef EPOLL_H_
#define EPOLL_H_

#include "Channel.hpp"
#include <vector>
#include "TimerGuard.hpp"

// 每一个Epoll对象是每一个EventLoop的属性，换句话说，Epoll仅仅作为EventLoop的属性存在
// 同时，一个Epoll对象拥有一个epoll_fd的资源，需要手动释放
class Epoll {
public:
    using channel_vector = std::vector<Channel*>;
    Epoll(EventLoop* _loop);
    ~Epoll();

public:
    void epoll_add(Channel* _channel, int op);
    void epoll_mod(Channel* _channel, int op);
    void epoll_del(Channel* _channel, int op);

    void handle_overtimer();
    void poll(channel_vector& v);

private:
    static const int MAXFDS = 1e5 + 1;
    int epoll_fd_;
    std::vector<struct epoll_event> event_array_;
    EventLoop* own_loop_;
    // std::shared_ptr<EventLoop> own_loop_;
    // TODO:add timer
    // TimerGuard<std::shared_ptr<Channel>> timer_guard_;
};


#endif