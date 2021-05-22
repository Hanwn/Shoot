#ifndef UOOLS_H_
#define UOOLS_H_
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>


int socket_bind_listen(int _port) ;

void handle_for_sigpipe() ;

int set_non_blocking(int _fd);

int readn(int fd, void *, int);

int readn(int, std::string&, bool&);

int readn(int, std::string&);

int writen(int, void*, int);

int writen(int, std::string&);

enum CONNECTION_STATE{ CONNECTED = 0, CONNECTING, DISCONNECTING, DISCONNECTED};

enum HTTP_METHOD {POST = 1, GET, HEAD};

enum HTTP_VERSION{HTTP_10 = 1, HTTP_11};

enum process_state {PARSE_URL = 1, PARSE_HEADERS, RECV_BODY, ANALYSIS, FIN};

enum url_state {URL_AGAIN = 1, URL_ERR, URL_SUCCESS};

#endif