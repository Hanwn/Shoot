#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include "EventLoop.hpp"
#include <memory>
#include "logger.h"
#include "EventLoopPool.hpp"
#include "TcpConnection.hpp"

class TCPServer {
public:
    using thread_callback = std::function<void(EventLoop*)>;
    TCPServer(EventLoop* _loop, std::string _thread_name, int port);
    ~TCPServer();

public:
    void start();
    void handle_new_conn();

private:
    EventLoop* loop_;
    std::string thread_name;
    std::unique_ptr<EventLoopPool> event_loop_pool_;    
    bool started_;
    int port_;
    int listen_fd_;
    std::shared_ptr<Channel> accpet_channel_;
    static const int MAXFDS = 100000;
    thread_callback cb_;
};



#endif