#ifndef FIXED_BUFFER_H_
#define FIXED_BUFFER_H_

#include <iostream>

template<int SIZE>
class FixedBuffer{
public:
    FixedBuffer();
    ~FixedBuffer() = default;

public:
    void append(const char* msg, size_t len);
    const char* data() const;
    int size() const;
    char* current();
    int avail() const;
    void add(size_t len);
    void reset();
    void bzero();

    //for test and debug function
    void display() {
        std::cout<<data_<<std::endl;
    }

private:
    const char* end() const;
    // QUESTION:template parameter as array length?
    char data_[SIZE];
    // cur_ point to the data end, not data_ end
    char* cur_;
};

const int small_buffer = 4000;
const int large_buffer = 4000*1000;

#endif