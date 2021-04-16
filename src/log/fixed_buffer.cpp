#include "fixed_buffer.h"


template<int SIZE>
FixedBuffer<SIZE>::FixedBuffer() :cur_(data_){}

template<int SIZE>
void FixedBuffer<SIZE>::append(const char* msg, size_t len) {
    if (avail() > static_cast<int>(len)) {
        memcpy(cur_, msg, len)
        cur_ += len;
    }
}


template<int SIZE>
const char* FixedBuffer<SIZE>::data() const {
    return data_;
}


template<int SIZE>
int FixedBuffer<SIZE>::size() const{
    return static_cast<int>(cur_ - data_);
}


template<int SIZE>
char* FixedBuffer<SIZE>::current() {
    return cur_;
}


template<int SIZE>
int FixedBuffer<SIZE>::avail() const {
    return static_cast<int>(end() - cur_);
}


template<int SIZE>
void FixedBuffer<SIZE>::add(size_t len) {
    cur_ += len;
}


template<int SIZE>
void FixedBuffer<SIZE>::reset() {
    cur_ = data_;
}


template<int SIZE>
void FixedBuffer<SIZE>::bzero() {
    memset(data_, 0, sizeof data_);
}


template<int SIZE>
const char* FixedBuffer<SIZE>::end() const {
    return data_ + sizeof data_;
}