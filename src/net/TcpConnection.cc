#include "TcpConnection.hpp"
#include "logger.h"

TCPConnection::TCPConnection(EventLoop* _loop, int _conn_fd) 
    : loop_(_loop)
    , channel_(new Channel(loop_, _conn_fd))
    , fd_(_conn_fd)
    , err_(false)
    , connection_state(CONNECTED)
    , http_method_(GET)
    , http_version(HTTP_11){
        channel_->set_read_callback(std::bind(&TCPConnection::handle_read, this));
        channel_->set_write_callback(std::bind(&TCPConnection::handle_write, this));
        // (std::bind(TCPConnection::handle_conn, this));
        channel_->set_close_callback(std::bind(&TCPConnection::handle_close, this));
        channel_->set_error_callback(std::bind(&TCPConnection::handle_err, this));
}


TCPConnection::~TCPConnection() {
    ::close(fd_);
}


void TCPConnection::handle_read() {
    LOG<<"handle_read";
    ssize_t read_len;
    char buf[1024];
    // ssize_t read_len = read(channel_->get_fd());
    read_len = read(channel_->get_fd(), buf, sizeof buf);
    if (read_len > 0) {
        //TODO:read data
        for (int i = 0; i < read_len; ++i) {
            std::cout<<buf[i];
        }
    }else if (read_len == 0) {
        handle_close();
    }else {
        handle_err();
    }
}

void TCPConnection::handle_write() {

}


void TCPConnection::handle_conn() {

}

void TCPConnection::handle_err() {

}

void TCPConnection::handle_close() {

}

void TCPConnection::new_event() {
    LOG<<"new conn";
    channel_->enable_read();
}