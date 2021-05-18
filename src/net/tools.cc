#include "tools.hpp"
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


int socket_bind_listen(int _port) {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == - 1) {
        ::close(listen_fd);
        return -1;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)_port);
    int val = bind(listen_fd, (struct sockaddr* )&server_addr, sizeof server_addr);
    if (val == -1) {
        ::close(listen_fd);
        return -1;
    }
    val = listen(listen_fd, 1024);
    if (val == -1) {
        ::close(listen_fd);
        return -1;
    }
    return listen_fd;
    
}

void handle_for_sigpipe() { 
    ::signal(SIGPIPE, SIG_IGN);
}

int set_non_blocking(int _fd) {
    int flags, s;
    flags = fcntl(_fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    flags |= O_NONBLOCK;
    s = fcntl(_fd, F_SETFL, flags);
    return 0;
}

int readn(int fd, void* buf, size_t n) {
    size_t nleft = n;
    ssize_t nread = 0;
    ssize_t read_cnt = 0;
    char* ptr = static_cast<char*>(buf);
    while (nleft > 0) {
        if ((nread = read(fd, buf, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
            } else if (errno == EAGAIN) {
                return read_cnt;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            break;
        }
        read_cnt += nread;
        nleft -= nread;
        ptr += nread;
        return read_cnt;
    }

}

int readn(int fd, std::string& _in_buf, bool& zero) {
    ssize_t nread = 0;
    int BUF_SIZE = 4096;
    ssize_t read_cnt = 0;
    while (true) {
        char _buf[BUF_SIZE];
        ::memset(_buf, 0, sizeof _buf);
        if ((nread = read(fd, _buf, BUF_SIZE)) < 0) {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN) {
                return read_cnt;
            } else {
                perror("read error");
                return -1;
            }
        }else if(nread == 0) {
            zero = true;
            // break;
            return 0;
        }
        read_cnt += nread;
        _in_buf += std::string(_buf, _buf + nread);
    }
    return read_cnt;
}

int readn(int fd, std::string& in_buf) {
    return 0;
}


int writen(int fd, std::string& _out_buf) {
    size_t nleft = _out_buf.size();
    ssize_t nwritten = 0;
    ssize_t write_cnt = 0;
    const char* ptr = _out_buf.c_str();
    while (nleft > 0){
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0) {
                if (errno == EINTR) {
                    nwritten = 0;
                    continue;
                } else if (errno == EAGAIN) {
                    break;
                } else {
                    return -1;
                }
            }
        }
        write_cnt += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    if (write_cnt == static_cast<int>(_out_buf.size())) {
        _out_buf.clear();
    }else{
        _out_buf = _out_buf.substr(write_cnt);
    }
    return write_cnt;
    
}

int writen(int fd, void* buf, int n) {
    size_t nleft = n;
    ssize_t nwritten = 0;
    ssize_t writen_cnt = 0;
    char * ptr = static_cast<char*>(buf);
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
                if (nwritten < 0) {
                if (errno == EINTR) {
                    nwritten = 0;
                    continue;
                } else if (errno == EAGAIN) {
                    nwritten = 0;
                    continue;
                } else {
                    return -1;
                }
            }
        }
        writen_cnt += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return writen_cnt;
}