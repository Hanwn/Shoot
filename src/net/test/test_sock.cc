#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
int main() {
    ::signal(SIGPIPE, SIG_IGN);
    int _port = 9618;

    int listen_fd = socket_bind_listen(_port);    

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof (client_addr);
    int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    char buf[1024];
    int ret = 0;
    while (true) {
        ret = read(client_fd, buf, sizeof buf);
        if (ret == 0) {
            break;
        }else if (ret < 0) {

        }
        for (int i = 0; i < ret; ++i) {
            buf[i] = toupper(buf[i]);
        }
        ret = ::write(client_fd, buf, ret);
    }
    ::close(listen_fd);
    ::close(client_fd);
    return 0;
}