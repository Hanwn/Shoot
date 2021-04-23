#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"
#include "epoll.h"

int Utils::set_nonblocking_io(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1) {
        return -1;
    }
    
    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1) {
        return -1;
    }
}

int Utils::socket_bind_listen(int port) {

    if (port <= 1024 || port >= 65535) {
        port = 9618;
    }
    int listen_fd = 0;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        return -1;
    }
    int optval = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)) == -1){
        return -1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        return -1;
    }

    if (listen(listen_fd, 1024) == -1) {
        return -1;
    }
    return listen_fd;
}

int Utils::read_conf() {
    //TODO:从配置文件中读取
    int port = 9618;
    return port;
}