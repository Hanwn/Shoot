#include "tools.hpp"
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>


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