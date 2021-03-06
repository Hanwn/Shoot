#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include "EventLoop.hpp"
#include <memory>
#include <functional>
#include <iostream>
#include <unistd.h>
#include "tools.hpp"
#include "http_parse.hpp"
#include "TimerGuard.hpp"
#include "Channel.hpp"
#include "EventLoop.hpp"

class EventLoop;
class Channel;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>{
public:
    using close_callback = std::function<void(std::shared_ptr<TCPConnection>)>;
    TCPConnection(EventLoop* _loop, int _conn_fd, std::shared_ptr<TimerGuard<TCPConnection>>);
    ~TCPConnection();

public:
    void set_close_callback(const close_callback& cb) { close_cb = cb; };

public:
    void reset();
    void new_event();
    EventLoop* get_loop();
    void destroy_conn();
    int get_fd() {return fd_;};

private:
    void handle_read();
    void handle_write();
    void handle_conn();
    void handle_err();
    void handle_close();

private:
    EventLoop* loop_;
    std::shared_ptr<Channel> channel_;
        std::shared_ptr<HTTPParse> http_parser;
    std::weak_ptr<TimerGuard<TCPConnection>> timer_;
    int fd_;
    std::string in_buf_;
    std::string out_buf_;
    bool err_;
    
    CONNECTION_STATE connection_state;
    HTTP_METHOD http_method_;
    HTTP_VERSION http_version;

    close_callback close_cb;
};


#endif
