#include "TcpConnection.hpp"
#include "logger.h"

TCPConnection::TCPConnection(EventLoop* _loop, int _conn_fd) 
    : loop_(_loop)
    , channel_(new Channel(loop_, _conn_fd))
    , fd_(_conn_fd)
    , err_(false)
    , connection_state(CONNECTING)
    , http_method_(GET)
    , http_version(HTTP_11){
        channel_->set_read_callback(std::bind(&TCPConnection::handle_read, this));
        channel_->set_write_callback(std::bind(&TCPConnection::handle_write, this));
        // (std::bind(TCPConnection::handle_conn, this));
        channel_->set_close_callback(std::bind(&TCPConnection::handle_close, this));
        channel_->set_error_callback(std::bind(&TCPConnection::handle_err, this));
        connection_state = CONNECTED;
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
        // for (int i = 0; i < read_len; ++i) {
        //     std::cout<<buf[i];
        // }
        // 需要在这里将数据读取完整，将整体分为get,post，请求
        // 需要通过content-length判断数据是否完整
        // 如果无法一次读取完整，则需要将已经读取的内容保存起来，然后再次读
        LOG<<"read data:"<<buf;

        // 如果读取完整，则需要往对端写数据
        handle_write();
    }else if (read_len == 0) {
        // 说明对端关闭
        handle_close();
    }else {
        if (errno == EINTR) {
        
        
        }else if (errno == EAGAIN) {
            // 如果读取不完整，并且出现了EAGAIN

        }else{
            handle_close();
        }
        handle_err();
    }
}

void TCPConnection::handle_write() {
    // 如果可以一次写完
    
    // 如果一次写不完，需要将channel_的EPOLLOPUT事件打开，同时将当前文件描述符注册到epoll中


    // 写完后，将channel_的EPOLLOUT事件关闭
}


void TCPConnection::handle_conn() {

}

void TCPConnection::handle_err() {

}

void TCPConnection::handle_close() {
    //assert_in_loop();
    connection_state = DISCONNECTED;
    channel_->disalbel_all();
    std::shared_ptr<TCPConnection> from_this(shared_from_this());
    close_cb(from_this);
}

void TCPConnection::destroy_conn() {
    //TODO:
    //assert
    if (connection_state == CONNECTED) {
        connection_state = DISCONNECTED;
        channel_->disalbel_all();
    }
    channel_->remove();
}

void TCPConnection::new_event() {
    LOG<<"new conn";
    channel_->enable_read();
}

EventLoop* TCPConnection::get_loop() {
    return loop_;
}