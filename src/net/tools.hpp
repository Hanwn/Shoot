#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int socket_bind_listen(int _port) {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(_port);
    bind(listen_fd, (struct sockaddr* )&server_addr, sizeof server_addr);
    listen(listen_fd, 1024);
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

enum CONNECTION_STATE{ CONNECTED = 0, DISCONNECTING, DISCONNECTED};

enum HTTP_METHOD {POST = 1, GET, HEAD};

enum HTTP_VERSION{HTTP_10 = 1, HTTP_11};

enum process_state {PARSE_URL = 1, PARSE_HEADERS, RECV_BODY, ANALYSIS, FIN};

enum url_state {URL_AGAIN = 1, URL_ERR, URL_SUCCESS};
