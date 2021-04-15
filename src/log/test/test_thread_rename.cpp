#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <time.h>



void* thread_func(void* p) {
    prctl(PR_SET_NAME, "test_e");
    usleep(100000000000);
    void* ret;
    pthread_exit(ret);
}


int main() {

    pthread_t th;
    int err = pthread_create(&th, nullptr, thread_func, nullptr);
    
    void * ret;
    pthread_join(th, &ret);
    return 0;
}