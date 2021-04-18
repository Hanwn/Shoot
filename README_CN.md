<div align=center><img src="https://github.com/Hanwn/Shoot/blob/dev/pic/logo.png?raw=true" width="200px" height="200px" ></div>

<h1 align=center>Shoot HTTP 服务器</h1>

- [1. :beginner:Introduce](#1-beginnerintroduce)
- [2. :sparkles:特点](#2-sparkles特点)
- [3. :construction:项目结构](#3-construction项目结构)
  - [3.1. :clipboard:日志模块](#31-clipboard日志模块)
  - [3.2. :watch:定时器模块](#32-watch定时器模块)
  - [3.3. :signal_strength:网络模块](#33-signal_strength网络模块)
- [4. :rocket:压测](#4-rocket压测)
- [5. :hourglass:TODO](#5-hourglasstodo)
- [6. :books:参考](#6-books参考)
# 1. :beginner:Introduce

Shoot 是一个在Linux下开发的高性能服务器，使用`C++11`开发，主要有三个模块，分别是日志模块、定时器模块和网络模块。


# 2. :sparkles:特点

# 3. :construction:项目结构

## 3.1. :clipboard:日志模块
![core struct](https://github.com/Hanwn/Shoot/blob/log/pic/log_core.png?raw=true)

- `Logger.cpp`
- `Logger_stream.cpp`
- `FixedBuffer.h`
- `async_logging.cpp`
- `log2file.cpp`
- `file2disk.cpp`

![PIC](https://github.com/Hanwn/Shoot/blob/log/pic/log_module1.png?raw=true)

使用**双缓冲**机制实现生产者-消费者模型。
- `producer`
  - `curr_buffer`:将**一批**日志文件写入到`curr_buffer`，如果`curr_buffer`满了，就会将当前的缓冲区加入到`buffers_`中，准备向磁盘写，同时会将`next_buffer`转为己用，如果`next_buffer`已经被用了，这时就会再一次申请缓冲区。
    :bell:注：**一批**的意思是1个`Logger`对象生存期间，往`buffer_`中写入的数据。
  - `next_buffer`:备用缓冲区，防止当前缓冲区已满；
  - `buffers_`:存储缓冲区的列表，准备写入磁盘；
- `consumer`
  - `write_buffers_`:当消费者要工作时，会把`curr_buffer`中的数据也拿过来，即使`curr_buffer`没有满，用消费者中的队列`write_buffer`与`buffers_`进行直接交换，对`buffers_`中的缓冲区进行写入`FILE* f`对应的缓冲区`Buffer_`，然后再进行写入磁盘；
  - `new_buffer1`:赋值给`curr_buffer`
  - `new_buffer2`:赋值给`next_buffer`

## 3.2. :watch:定时器模块


## 3.3. :signal_strength:网络模块

# 4. :rocket:压测





# 5. :hourglass:TODO


# 6. :books:参考

