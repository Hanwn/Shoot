#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include "EventLoop.hpp"
#include <memory>
#include <iostream>
#include <unistd.h>
#include "tools.hpp"


class TCPConnection : public std::enable_shared_from_this<TCPConnection>{
public:
    TCPConnection(EventLoop* _loop, int _conn_fd);
    ~TCPConnection();

public:
    void reset();
    void new_event();

private:
    void hanle_read();
    void handle_write();
    void handle_conn();
    void handle_err();
    void handle_close();

private:
    EventLoop* loop_;
    std::shared_ptr<Channel> channel_;
    int fd_;
    std::string in_buf_;
    std::string out_buf_;
    bool err_;
    
    CONNECTION_STATE connection_state;
    HTTP_METHOD http_method_;
    HTTP_VERSION http_version;
};


#endif