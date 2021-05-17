#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>
#include <sys/epoll.h>
#include "EventLoop.hpp"
#include "logger.h"

enum class channelStatus{
    NEW = 1,
    ADD = 3,
    DEL = 2
};


// Channel 的生命周期由TCPConnection控制
// Channel 不拥有任何资源，无需在析构函数中释放
class EventLoop;
class Channel {
public:
    using callback = std::function<void()>;
    Channel(EventLoop* _loop, int fd);
    ~Channel();

public:

    void set_read_callback(callback cb) {
        read_handler_ = cb;
    }

    void set_write_callback(callback cb) {
        write_handler_ = cb;
    }

    void set_error_callback(callback cb) {
        LOG<<"channel error callback";
        error_handler_ = cb;
    }

    void set_close_callback(callback cb) {
        close_handler_ = cb;
    }

    void set_revents(int _revent) {
        revent_ = _revent;
    }

    void enable_read();
    void enable_write();
    void disable_read();
    void disable_write();
    void disalbel_all();

public:
    void set_events(int op);
    int get_events();
    int get_fd();
    channelStatus get_status();
    void set_status(channelStatus _status);

public:

    void update();
    void remove();
    // STAR: 这个函数作为一个channel的回调，非常重要
    void handle_events();
private:
    EventLoop* loop_;
    int event_;
    int revent_;
    int fd;

    static const int none_event;
    static const int read_event;
    static const int write_event;

    channelStatus status;

    callback read_handler_;
    callback write_handler_;
    callback error_handler_;
    callback close_handler_;
};


#endif