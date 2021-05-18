#include "TcpConnection.hpp"
#include "logger.h"
#include "http_parse.hpp"

char favicon[] = {};



TCPConnection::TCPConnection(EventLoop* _loop, int _conn_fd, std::shared_ptr<TimerGuard<TCPConnection>> _timer)
    : loop_(_loop)
    , channel_(new Channel(loop_, _conn_fd))
    , timer_(_timer)
    , http_parser(new HTTPParse())
    , fd_(_conn_fd)
    , err_(false)
    , connection_state(CONNECTING)
    , http_method_(GET)
    , http_version(HTTP_11){
        channel_->set_read_callback(std::bind(&TCPConnection::handle_read, this));
        // channel_->set_write_callback(std::bind(&TCPConnection::handle_write, this));
        // (std::bind(TCPConnection::handle_conn, this));
        channel_->set_close_callback(std::bind(&TCPConnection::handle_close, this));
        channel_->set_error_callback(std::bind(&TCPConnection::handle_err, this));
        connection_state = CONNECTED;
}


TCPConnection::~TCPConnection() {
    // LOG<<"::close connection";
    // handle_close();
    ::close(fd_);
}


void TCPConnection::handle_read() {
    // LOG<<"handle_read";
    ssize_t read_len;
    char buf[1024];
    ::memset(buf, 0, sizeof buf);
    // ssize_t read_len = read(channel_->get_fd());
    // STAR:默认对于请求操作是可以一次读完的，即确保可以一次读完
    read_len = read(channel_->get_fd(), buf, sizeof buf);
    if (read_len > 0) {
        //TODO:read data
        // for (int i = 0; i < read_len; ++i) {
        //     std::cout<<buf[i];
        // }
        // 需要在这里将数据读取完整，将整体分为get,post，请求
        // LOG<<"read data:"<<buf;

        /*
        if (!http_parser->parse_header(buf, ::strlen(buf)) ) {
            // 对方发来的不是一个http请求，可以立即关闭
            // LOG<<"close connection : http->parser->header";
            handle_close();
        }
        std::string& _filename = http_parser->get_filename();
        // LOG<<"request file : "<<_filename;
        // LOG<<"request data:"<< buf;
        if (_filename.size() && _filename != "favicon.ico") {
            // LOG<<"req data : favicon and filename"<<_filename;
            handle_err();
        }
        // if method is post
        if (http_parser->get_method() == HTTP_METHOD::POST) {
            http_parser->parse_body();
        }
        */

        // 读取完整，则需要往对端写数据
        handle_write();
        //TODO:加入到定时器中
        // handle_err();
        // handle_close();
    }else if (read_len == 0) {
        // 说明对端关闭
        LOG<<"peer close";
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
    std::string  header_buf;
    // char output_buf[4096];
    // ::memset(output_buf, 0, sizeof output_buf);
    header_buf += "HTTP/1.1 200 ok\r\n";
    header_buf += "Connection:Close\r\n";
    // body
    std::string content_buf;
    // HEAD
    if (http_parser->get_filename() == "favicon.ico") {
        header_buf += "Content-Type: image/png\r\n";
        header_buf += "Contetn-Length:" + std::to_string(sizeof favicon) + "\r\n";
        header_buf += "Shoot Server\r\n";
        header_buf += "\r\n";
        out_buf_ += header_buf;
        out_buf_ += std::string(favicon, favicon + sizeof favicon);
    }else {
        std::string body_buf = " \
        <html>\
            <title>Shoot Web Server</title>\
            <body>\
                <h1 align=\"center\">Shoot Web Server</h1>\
                <div align=\"center\">\
                    Shoot web server is a web server in linux. This projet is in the <a href=\"https//www.github.com/Hanwn/Shoot.com\">repo</a>.\
                </div>\
            </body>\
        </html>";
        header_buf += "Content-Type: text/html\r\n";
        // TODO:connection is alive;
        // header_buf += "Connection: Keep-Alive\r\nKeep-Alive: timeout=6\r\n";
        header_buf += "Content-Length:" + std::to_string(body_buf.size()) + "\r\n";
        header_buf += "Server: Shoot Web server\r\n";
        header_buf += "\r\n";
        content_buf = header_buf + body_buf;
    }
    // verseion 1 如果可以一次写完
    char send_buf[4096];
    ::memset(send_buf, 0, sizeof send_buf);
    ::sprintf(send_buf, "%s", content_buf.c_str());
    //TODO: write_back() function
    ::write(this->fd_, send_buf, ::strlen(send_buf));
    // version 2 如果一次写不完，需要将channel_的EPOLLOPUT事件打开，同时将当前文件描述符注册到epoll中
    


    // 写完后，将channel_的EPOLLOUT事件关闭

    // 再次加入定时器
    /*
    {
        auto tmp_timer = timer_.lock();
        tmp_timer->add_time_node(shared_from_this(),20);
    }
    */
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
    if (from_this) {
        close_cb(from_this);
    }
}

void TCPConnection::destroy_conn() {
    //TODO:
    //assert
    // LOG<<"destory connection";
    if (connection_state == CONNECTED) {
        connection_state = DISCONNECTED;
        channel_->disalbel_all();
    }
    channel_->remove();
}

void TCPConnection::new_event() {
    // LOG<<"new conn";
    // 加入定时器
    auto tmp_timer = timer_.lock();
    // tmp_timer->add_time_node(shared_from_this(), 20);
    channel_->enable_read();
}

EventLoop* TCPConnection::get_loop() {
    return loop_;
}
