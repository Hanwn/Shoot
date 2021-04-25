#ifndef EPOLL_H_
#define EPOLL_H_

#include "Channel.hpp"
#include <vector>
#include "TimerGuard.hpp"

// 每一个Epoll对象是每一个EventLoop的属性，换句话说，Epoll仅仅作为EventLoop的属性存在
class Epoll {
public:
    Epoll();
    ~Epoll() = default;

public:
    void epoll_add(std::shared_ptr<Channel> _channel, int timeout);
    void epoll_mod(std::shared_ptr<Channel> _channel, int timeout);
    void epoll_del(std::shared_ptr<Channel> _channel);

    void handle_overtimer();
    std::vector<std::shared_ptr<Channel>> poll();

private:
    static const int MAXFDS = 1e5 + 1;
    int epoll_fd_;
    std::vector<Channel> event_array_;
    TimerGuard<std::shared_ptr<Channel>> timer_guard_;
};


#endif