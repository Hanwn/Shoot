#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


static void* cleanup_function(void* p) {
    puts(p);
}

static void* func(void* p) {
    puts("thread is running");
    pthread_cleanup_push(cleanup_function,"cleanup push 1");
    pthread_cleanup_push(cleanup_function,"cleanup push 2");
    pthread_cleanup_push(cleanup_function,"cleanup push 3");
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_exit(NULL);

}

int main() {
    puts("begin!");
    pthread_t tid;
    int err;
    err = pthread_create(&tid,NULL,func,NULL);
    if (err) {
        fprintf(stderr,"pthread_create() : %s\n",strerror(err));
        exit(1);
    }
    pthread_join(tid,NULL);
    puts("end!");
    return 0;
}