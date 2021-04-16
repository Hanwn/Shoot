#ifndef FIXED_BUFFER_H
#define FIXED_BUFFER_H

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

private:
    const char* end() const;
    // QUESTION:template parameter as array length?
    char data_[SIZE];
    // cur_ point to the data end, not data_ end
    char* cur_;
};


#endif