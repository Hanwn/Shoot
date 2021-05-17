#include "TcpConnection.hpp"
#include "logger.h"
#include "http_parse.hpp"

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
    // STAR:默认对于请求操作是可以一次读完的，即确保可以一次读完
    read_len = read(channel_->get_fd(), buf, sizeof buf);
    if (read_len > 0) {
        //TODO:read data
        // for (int i = 0; i < read_len; ++i) {
        //     std::cout<<buf[i];
        // }
        // 需要在这里将数据读取完整，将整体分为get,post，请求
        LOG<<"read data:"<<buf;

        std::shared_ptr<HTTPParse> http_parser(new HTTPParse());
        http_parser->parse_header();
        // if method is post
        if (http_parser->get_method() == HTTP_METHOD::POST) {
            http_parser->parse_body();
        }
        // 读取完整，则需要往对端写数据
        handle_write();
        handle_err();
        handle_close();
    }else if (read_len == 0) {
        // 说明对端关闭
        handle_close();
    }else {
        if (errno == EINTR) {
            // 当前系统调用的时间较长，但是信号(signal)会打断时间较长的系统调用
            // 解决方法：应该重启当前事件，所以本项目将当前fd再次加入到epoll中进行监听
            loop_->run_in_loop(std::bind(&Channel::enable_read, channel_));
        }else if (errno == EAGAIN) {
            // 出现了EAGAIN，需要重启当前读事件
            loop_->run_in_loop(std::bind(&Channel::enable_read, channel_));
        }else{
            handle_close();
        }
        handle_err();
    }
}

void TCPConnection::handle_write() {
    // header

    // body
    
    // HEAD

    // verseion 1 如果可以一次写完
    
    // version 2 如果一次写不完，需要将channel_的EPOLLOPUT事件打开，同时将当前文件描述符注册到epoll中
    


    // 写完后，将channel_的EPOLLOUT事件关闭
}


void TCPConnection::handle_conn() {

}

void TCPConnection::handle_err() {
    std::string body_buf,header_buf;
    std::string send_buf;
    body_buf += "<html><title> error </title>";
    body_buf += "<body bgcolor=\"ffffff\">";
    body_buf += std::to_string(404);
    body_buf += "<hr><em>Web Server</em>\n</body></html>";

    header_buf += "HTTP/1.1" + std::to_string(404) + "\r\n";
    header_buf += "Content-type:text/html\r\n";
    header_buf += "Connnection:Close\r\n";
    header_buf += "Content-Length: " + std::to_string(body_buf.size()) + "\r\n";
    header_buf += "Server: Shoot Web Server\r\n";
    header_buf += "\r\n";

    send_buf = header_buf + body_buf;
    char send_buffer[4096];

    sprintf(send_buffer, "%s", send_buf.c_str());
    //TODO: write_back() function
    ::write(this->fd_, send_buffer, strlen(send_buffer));
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
