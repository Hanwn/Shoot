#include "create_thread.h"

static void* func(void*p) {
    puts("thread is working");
    pthread_exit(NULL);
}

int main() {
    pthread_t tid;
    int err;
    puts("beging()!");
    err = pthread_create(&tid,NULL,func,NULL);
    if (err) {
        fprintf(stderr,"pthread_create():%s\n",strerror(err));
        exit(1);
    }
    pthread_join(tid,NULL);
    puts("end");
    exit(0);

    return 0;
}