#ifndef HEAP_H_
#define HEAP_H_

#include <pthread.h>
#include "utils.h"
#define HEAPSIZE 1024

class priority_queue_ {
public:
    priority_queue_();
    ~priority_queue_();

public:
    int pq_insert(time_n_t*);
    bool pq_is_empty();
    void* pq_front();
    void pq_pop();

public:
    pthread_mutex_t mut;
private:
    void up(int);
    void down(int);
    time_n_t** heap;
    int size;

};

using pq_t = class priority_queue_;


#endif