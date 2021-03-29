#include "http.h"
#include <iostream>


void Http::do_request(void* ptr) {
    // do_request函数在一开始执行的时候一定是返回EPOLLIN任务
    // 在读完来自客户端的内容后，应该把相应的文件描述符置为可写

    // 最简单的方案是考虑从客户端读取一次性可以读完，也可以一次性写完
    // 那么读完之后就可以写，写完之后就可以读，因而，在写完后将文件描述符置为EPOLLIN状态

    //TODO:关闭文件描述符，解析http报文头,返回数据
    // TODO:从堆上删除当前节点
    http_request_t* request = (http_request_t*)ptr;
    //request->time_->time_n_del(request);
        http_parse(ptr);
   /*
    if (request->resolve == 1) {
        // 可写事件

        //可读事件
    } else {
        // http头尚未解析的时候会进入此函数执行
        //parse函数在修改ptr->fd的时候应该注意监视其可读可写(大文件)状态
        std::cout<<"do_request"<<"\n";
        http_parse(ptr);
        request->resolve = 1;
    }
    */
   /*
    http_request_t* request = (http_request_t*)ptr;
    
    int fd = request->fd;
    char read_buf[1000];
    read(fd, read_buf, sizeof(read_buf));
    std::cout<<read_buf<<"\n";
    char buf[] = "HTTP/1.0 200 \nContent-Type: text/html\nContent-Length: 5\n\n helo";
    int ret = sizeof (buf);
    write(fd, buf, ret);
    //close(fd);
    //delete request;
    do{

    }while (0);
    */
    
}

void Http::http_parse(void* ptr) {
    //TODO:解析http头
    http_request_t* request = (http_request_t*)ptr;
    int fd = request->fd;
    state_ &state = request->state;
    int error_ = 0;
    int flag_ = 0;
    do {
        int read_count = read_(fd, request->inbuf_, flag_);
        if (read_count < 0) {
            //出错
            error_ = 1;
            do_error();
            break;
            //返回400错误
        }else if (flag_) {
            //客户端关闭
            error_ = 1;
            break;
        }

        //解析请求行
        if (state == state_parse_uri) {
            parse_uri_flag flag = parse_uri(request);
            if (flag == parse_uri_again) {
                //TODO:请求行太长
                break;
            }else if (flag == parse_uri_error) {
                request->inbuf_.clear();
                error_ = 1;
                do_error();
                break;
            }else{
                state = state_parse_header;
            }
        }
        if (state == state_parse_header) {
            parse_header_flag flag = parse_headers(request);
            if (flag == parse_header_again) {
                //TODO:请求行太长，一般不会出现这样的情况
                break;
            }else if (flag == parse_header_error) {
                error_ = 1;
                do_error();
                break;
            }else{
                // 默认是get请求方法
                state = state_parse_analysis;
                if (request->method == method_post) {
                    state = state_parse_body;
                }
            }
        }
        if (state == state_parse_body) {

        }
        if (state == state_parse_analysis) {
            // 处理get请求
            // TODO:处理大文件发送任务
            parse_analysis_flag flag = parse_analysis(request);
            if (flag == parse_analysis_again || flag == parse_analysis_error) {
                error_ = 1;
                break;
            }else {
                state = state_parse_fin;
                break;
            }
        }

    }while (false);
    if (!error_) {
        if (request->outbuf_.size() > 0) {
            //TODO:在传输大文件的时候需要将这个文件描述符再次加入epoll监听，以备下次可写
            //需要在while中写好返回信息
            //write之后存在两种情况，1-写完了，需要关闭；2-没有写完，需要等待该文件描述符可写可读
            std::cout<<"no error\n";
            http_write(fd, request->outbuf_);
            //更新时间，将此文件描述符对应的插入时间设置为0，同时设置文件描述符为可读状态
            //request->time_->time_n_add(request, TIMEOUT_DEFAULT, close_conn);
            //request->epoll->epoll_event_mod(fd, request, (EPOLLIN | EPOLLET | EPOLLONESHOT));
        }
    } else {
        // 出现了错误，调用close函数，关闭已有的连接
        if (!flag_) {
            std::cout<<"error\n";
            if (request->outbuf_.size() > 0) {
                http_write(fd, request->outbuf_);
            }
            //需要对request进行拷贝构造
            //request->time_->time_n_add(request, TIMEOUT_DEFAULT, close_conn);
            //request->epoll->epoll_event_mod(fd, request, (EPOLLIN | EPOLLET | EPOLLONESHOT));
            //如果是keep-alive，
        } else {
            // 在这个地方进行释放request，出出现一个问题，当进入超时处理函数时，也会对这个节点进行释放request，所以会存在两次delete的问题，因而，最好的办法是在这个地方进行超时设置，将req中的time_n_t节点deleted属性设置为真
            // close_conn(request);
        }
    }
    //close_conn(request);
    std::cout<<"endding\n";
}

parse_uri_flag Http::parse_uri(http_request_t* request) {
    std::string &str = request->inbuf_;
    std::string copy = str;
    size_t pos = str.find('\r', 0);
    if (pos < 0) {
        //TODO:不存在\r标识符，如何处理
    }
    std::string request_line = str.substr(0, pos);

    int pos_g = request_line.find("GET");
    int pos_p = request_line.find("POST");

    if (pos_g >= 0) {
        pos = pos_g;
        request->method = method_get;
    }else if (pos_p >= 0) {
        pos = pos_p;
        request->method = method_post;
    }

    // 解析filename
    pos = request_line.find("/", pos);
    if (pos < 0) {
        request->uri = "index.html";
        request->http_v = HTTP_11;
        return parse_uri_success;
    } else {
        size_t pos_ = request_line.find(" ", pos);
        //pos_指向的位置是位于uri和http版本之间的空格
        if (pos_ < 0) {
            return parse_uri_error;
        } else {
            if (pos_ - pos > 1) {
                request->uri = request_line.substr(pos + 1, pos_ - pos - 1);
                size_t pos__ = request->uri.find("?");
                if (pos__ >= 0) {
                    request->uri = request->uri.substr(0, pos__);
                }
            }else {
                request->uri = "index.html";
            }
        }
        pos = pos_;
    }
    pos = request_line.find("/", pos);

    if (pos < 0) {
        return parse_uri_error;
    }else {
        if (request_line.size() - pos <= 3) {
            return parse_uri_error;
        } else {
            std::string tmp = request_line.substr(pos + 1, 3);
            if (tmp == "1.0") {
                request->http_v = HTTP_10;
            }else if (tmp == "1.1") {
                request->http_v = HTTP_11;
            }else {
                return parse_uri_error;
            }
        }
    }
    return parse_uri_success;
}

parse_header_flag Http::parse_headers(http_request_t* request) {
    /*
    std::string& str = request->inbuf_;
    TODO:解析出header信息
    for (int i = 0; i < str.size(); ++i) {

    }
    */
    return parse_header_success;
}

parse_analysis_flag Http::parse_analysis(http_request_t* request) {
    std::string header;
    header += "HTTP/1.0 200 OK\r\n";
    //TODO: keep-alive
    size_t dot_pos = request->uri.find(".");
    std::string file_type;
    if (dot_pos < 0) {
        file_type = "text/html";
    }else{
        file_type = "text/html";//get_mime_type
    }

    //TODO:路由设计，打算作为独立出一个类
    if (request->uri == "hello") {
        request->outbuf_ = "HTTP/1.0 200 \nContent-Type: text/html\nContent-Length: 5\n\n helo";
        return parse_analysis_success;
    }
    if (request->uri == "favicon.ico") {
        /*
        header += "Content-Type: image/png\r\n";
        header += "Content-Length: " + std::to_string(sizeof (favicon)) + "\r\n";
        header += "Server: Shoot Http Server\r\n";
        header += "\r\n";
        request->outbuf_ += header;
        request->outbuf_ += std::string(favicon, favicon + sizeof(favicon));
        */
        return parse_analysis_success;
    }

    struct stat sbuf_;
    //TODO: stat 函数的作用
    if (stat(request->uri.c_str(), &sbuf_) < 0) {
        header.clear();
        do_error();
        return parse_analysis_error;
    }
    header += "Content-Type: " + file_type + "\r\n";
    header += "Content-Length: " + std::to_string(sbuf_.st_size) + "\r\n";
    header += "Server: Shoot Http Server\r\n";
    //写完http头部信息
    //必须有下一行
    header += "\r\n";

    request->outbuf_ += header;

    int src_fd = open(request->uri.c_str(), O_RDONLY, 0);

    if (src_fd < 0) {
        request->outbuf_.clear();
        do_error();
        return parse_analysis_error;
    }

    //TODO:mmap函数的作用
    void* ret = mmap(nullptr, sbuf_.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
    close(src_fd);

    if (ret == (void*)-1) {
        munmap(ret, sbuf_.st_size);
        request->outbuf_.clear();
        do_error();
        return parse_analysis_error;
    }

    //地址转换
    char* src_addr = static_cast<char*>(ret);
    request->outbuf_ += std::string(src_addr, src_addr + sbuf_.st_size);
    munmap(ret, sbuf_.st_size);
    return parse_analysis_success;
}

void Http::http_write(int fd, std::string& outbuf) {
    if (write_(fd, outbuf) < 0) {
        perror("writen");
    }
    if (outbuf.size() > 0) {
        std::cout<<"----->\n";
        // 加入监听，等待可写事件的发生
    }
}

int Http::write_(int fd, std::string& outbuf) {
    ssize_t left = outbuf.size();
    ssize_t written = 0;
    ssize_t write_count = 0;
    const char* ptr = outbuf.c_str();
    while (left > 0) {
        if ((written = write(fd, ptr, left)) <= 0) {
            // 出错或者客户端关闭
            if (written < 0) {
                if (errno == EINTR) {
                    written = 0;
                    continue;
                }else if (errno == EAGAIN) {
                    break;
                }else{
                    return -1;
                }
            }
        }
        write_count += written;
        left -= written;
        ptr += written;
    }
    if (write_count == static_cast<int>(outbuf.size())) {
        outbuf.clear();
    }else {
        outbuf = outbuf.substr(write_count);
    }
    return write_count;
}

int Http::read_(int fd, std::string& inbuf, int& flag) {
    ssize_t nread = 0;
    ssize_t read_count = 0;
    while (true) {
        char buf[BUFSIZE];
        if ((nread = read(fd, buf, BUFSIZE)) < 0) {
            // 读取出错，分别是被信号打断的系统调用和EAGAIN错误
            if (errno == EINTR) {
                continue;
            }else if(errno == EAGAIN) {
                return read_count;
            }else{
                perror("read error");
                return -1;
            }
        }else if (nread == 0) {
            // 客户端连接关闭
            flag = 1;
            break;
        }
        read_count += nread;
        inbuf += std::string(buf, buf + nread);
    }
    return read_count;
}

void Http::do_error() {

}

void Http::close_conn(http_request_t* request) {
    std::cout<<"close_conn\n";
    //epoll_ctl()
    close(request->fd);
    delete request;
}