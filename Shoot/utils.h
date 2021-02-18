#ifndef UTILS_H_
#define UTILS_H_

class Utils{
public:
    static int set_nonblocking_io(int);
    static int socket_bind_listen(int);
    static int read_conf();

};

#endif