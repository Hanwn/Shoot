#include "async_logging.h"

AsyncLogging::AsyncLogging(std::string _filename, int _flush_freq = 3 )
    : flush_freq(_flush_freq) ,
      running(false),
      filename_(_filename),
      thread_(std::bind(&AsyncLogging::thread_func, this), "Logger"),
      mutex_(),
      cond_(mutex_),
      curr_buffer_(new buffer),
      next_buffer_(new buffer),
      buffers_(),
      latch_(1) {
          curr_buffer_->bzero();
          next_buffer_->bzero();
          buffers_.reserve(16);
      }

AsyncLogging::~AsyncLogging() {
    if (running) stop();
}


void AsyncLogging::stop() {
    running = false;
    cond_.notify();
    thread_.join();
}


void AsyncLogging::start() {
    running = true;
    thread_.start();
    // when thread_fun is running, this thread will be waked up
    latch_.wait();
}


// Productor
// this function will be executed when runing in Logger disconstructor
void AsyncLogging::append(const char* msg, int len) {
    MutexLockGuard lock(mutex_);
    if (curr_buffer_->avail() > len) {
        curr_buffer_->append(msg, len);
    } else {
        buffers_.push_back(curr_buffer_);
        curr_buffer_.reset();
        if (next_buffer_) {
            curr_buffer_ = std::move(next_buffer_);
            //DONE:当move之后，next_buffer 已经不存在，所以使用next_buffer的意义是什么?
        } else {
            curr_buffer_.reset(new buffer);
        }
        curr_buffer_->append(msg, len);
        cond_.notify();
    }
}

// Consumer
void AsyncLogging::thread_func() {
    // void AsyncLoggint::start() function will finish;
    latch_.count_down();
    Log2File output(filename_);
    //smart pointer
    buffer_ptr new_buffer1(new buffer);
    buffer_ptr new_buffer2(new buffer);
    new_buffer1->bzero();
    new_buffer2->bzero();

    buffer_vector buffers_to_write;
    buffers_to_write.reserve(16);

    while (running) {
        
        //  code block 将生产者队列中(buffers_)的元素全部移动到消费者队列(buffers_to_write)中
        // 使用这样的方式可以尽量减小临界区，达到优化目的
        {
            MutexLockGuard lock(mutex_);
            // BUG:if or while?
            if (buffers_.empty()) {
                cond_.wait_seconds(flush_freq);
            }
            buffers_.push_back(curr_buffer_);
            curr_buffer_.reset();
            // 将curr_buffer_添加到待写vector中之后，需要使用new_buffer1重新赋值
            curr_buffer_ = std::move(new_buffer1);
            buffers_to_write.swap(buffers_);
            // 如果next_buffer_被使用了，则需要用消费者中的new_buffer重新赋值
            if (!next_buffer_) {
                next_buffer_ = std::move(new_buffer2);
            }
        }


        // TODO: 这是muduo代码中防止日志太大，而使用的一种处理方式，将多的日志丢掉
        // ，然后向日志中写入日志太多
        // 是否可以优化和改进
        if (buffers_to_write.size() > 25) {
            buffers_to_write.erase(buffers_to_write.begin() + 2, buffers_to_write.end());
        }

        for (int i = 0; i < buffers_to_write.size(); ++i) {
            // 会把缓冲区的日志文件写入文件的缓冲区
            output.append(buffers_to_write[i]->data(), buffers_to_write[i]->size());
        }

        if (buffers_to_write.size() > 2) {
            buffers_to_write.resize(2);
        }

        if (!new_buffer1) {
            new_buffer1 = buffers_to_write.back();
            buffers_to_write.pop_back();
            new_buffer1->reset();
        }

        if (!new_buffer2) {
            new_buffer2 = buffers_to_write.back();
            buffers_to_write.pop_back();
            new_buffer2->reset();
        }
        buffers_to_write.clear();
        output.flush();
    }
    output.flush();
}