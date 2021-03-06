#ifndef FIXED_BUFFER_H_
#define FIXED_BUFFER_H_

#include <iostream>
#include <string.h>

template <int SIZE>
class FixedBuffer {
public:
    FixedBuffer() : cur_(data_) {}

	~FixedBuffer() = default;

	void append(const char* buf, size_t len) {
		if (avail() > static_cast<int>(len)) {
			memcpy(cur_, buf, len);
			cur_ += len;
		}
  	}

	const char* data() const { return data_; }
	int size() const { return static_cast<int>(cur_ - data_); }

	char* current() { return cur_; }
	int avail() const { return static_cast<int>(end() - cur_); }
	void add(size_t len) { cur_ += len; }

    void reset() { cur_ = data_; }
    void bzero() { memset(data_, 0, sizeof data_); }
     //for test and debug function
    void display() {
         std::cout<<data_<<std::endl;
    }
private:
	const char* end() const { return data_ + sizeof data_; }

    // QUESTION:template parameter as array length?
	char data_[SIZE];
	char* cur_;
};

const int small_buffer = 4000;
const int large_buffer = 4000*1000;

#endif