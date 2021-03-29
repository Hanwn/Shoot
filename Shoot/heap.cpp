#include "heap.h"
#include <algorithm>

priority_queue_::priority_queue_() {
    //new 一个指针数组
    this->heap = new time_n_t*[HEAPSIZE];
    this->size = 0;
    pthread_mutex_init(&this->mut, nullptr);
}

priority_queue_::~priority_queue_() {
    for (int i = 0; i < HEAPSIZE; ++i) {
        if (this->heap[i]) {
            delete this->heap[i];
        }
    }
    delete []this->heap;
    pthread_mutex_destroy(&this->mut);
}

int priority_queue_::pq_insert(time_n_t* ptr) {
    pthread_mutex_lock(&mut);
    heap[++size] = ptr; 
    this->up(size);
    pthread_mutex_unlock(&mut);
}

void priority_queue_::up(int pos) {
    while (pos > 1) {
        if (heap[pos]->key < heap[pos/2]->key) {
            std::swap(heap[pos], heap[pos/2]);
            pos /= 2;
        }else{
            break;
        }
    }
}

bool priority_queue_::pq_is_empty() {
    return this->size == 0;
}

void priority_queue_::down(int pos) {
    int left = pos * 2;
    while (left <= size) {
        if (left < size && heap[left]->key > heap[left + 1]->key) {
            left++;
        }
        if (heap[left]->key < heap[pos]->key) {
            std::swap(heap[left], heap[pos]);
            pos = left, left = pos * 2;
        } else{
            break;
        }
    }
}

void priority_queue_::pq_pop() {
    pthread_mutex_lock(&this->mut);
    heap[1] = heap[size--];
    down(1);
    pthread_mutex_unlock(&this->mut);
}

void* priority_queue_::pq_front() {
    return this->heap[1];
}