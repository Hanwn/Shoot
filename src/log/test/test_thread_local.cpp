#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <time.h>
#include <linux/unistd.h>
// #include "../current_thread.h"
#include <typeinfo>
using namespace std;

pthread_mutex_t mutex;

string thread_name_;

void* thread_func(void * para) {
    int pit = static_cast<int>(::syscall(SYS_gettid));
    thread_name_ += to_string(pit);
    pthread_mutex_lock(&mutex);
    cout<<thread_name_<<endl;
    pthread_mutex_unlock(&mutex);
    void* ret;
    pthread_exit(ret);
}


int main() {
    pthread_mutex_init(&mutex, nullptr);
    const int thread_nums = 10;
    pthread_t ts[thread_nums];
    void * para = nullptr;
    for (int i = 0; i < thread_nums; ++i) {
        pthread_create(ts + i, nullptr, thread_func, para);
    }

    for (int i = 0; i < thread_nums; ++i) {
        void* p;
        pthread_join(ts[i], &p);
    }
    pthread_mutex_destroy(&mutex); 
    return 0;
}