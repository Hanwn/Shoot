#ifndef ASYNC_LOGGING_H_
#define ASYNC_LOGGING_H_

#include "fixed_buffer.h"
#include "Thread.h"
#include "CountDownLatch.h"
#include "Condition_.h"
#include <vector>
#include <memory>
#include <iostream>
#include "log2file.h"

class AsyncLogging{
public:
    AsyncLogging(std::string _filename, int _flush_freq = 3);
    ~AsyncLogging();

public:
    void append(const char* msg, int len);
    void start();
    void stop();

private:
    void thread_func();
    // DONE:small_buffer to large_buffer??
    using buffer = FixedBuffer<large_buffer>;
    using buffer_vector = std::vector<std::shared_ptr<buffer>> ;
    using buffer_ptr = std::shared_ptr<buffer>;

    const int flush_freq;
    bool running;
    std::string filename_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    buffer_ptr curr_buffer_;
    buffer_ptr next_buffer_;
    buffer_vector buffers_;
    CountDownLatch latch_;
};

#endif