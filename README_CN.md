<div align=center><img src="https://github.com/Hanwn/Shoot/blob/dev/pic/logo.png?raw=true" width="200px" height="200px" ></div>

<h1 align=center>Shoot HTTP 服务器</h1>

- [1. :beginner:Introduce](#1-beginnerintroduce)
- [2. :sparkles:特点](#2-sparkles特点)
- [3. :construction:项目结构](#3-construction项目结构)
  - [3.1. :clipboard:日志模块](#31-clipboard日志模块)
  - [3.2. :watch:定时器模块](#32-watch定时器模块)
  - [3.3. :signal_strength:网络模块](#33-signal_strength网络模块)
    - [3.3.1. 建立一条连接](#331-建立一条连接)
    - [3.3.2. 销毁一条连接](#332-销毁一条连接)
- [4. :rocket:压测](#4-rocket压测)
  - [4.1. :computer:机器1](#41-computer机器1)
    - [4.1.1. :link:短连接](#411-link短连接)
    - [4.1.2. :link:长连接](#412-link长连接)
  - [4.2. :computer:机器2](#42-computer机器2)
    - [4.2.1. :link:短连接](#421-link短连接)
    - [4.2.2. :link:长连接](#422-link长连接)
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

![](https://github.com/Hanwn/Shoot/blob/net/pic/shoot_net.png?raw=true)

### 3.3.1. 建立一条连接
1. 当一个客户端与服务器建立连接，首先会与本项目中的`main thread`建立连接，连接建立后（三次握手），会`main thread`
会调用主线程的回调函数创建一个`Connection`对象；然后会把这个对象分配给其他的线程`sub thread`，如上图中③所示，
在`sub thread`中每一个`Connection`对象的读写事件，都接受`EPOLL`的监听。

2. 当客户端有数据到来时，`EPOLL`会激活对应的事件，将激活的事件（event）将内核转移到用户空间，然后进行处理
相应事件的回调函数

### 3.3.2. 销毁一条连接

# 4. :rocket:压测

```shell
ab -n 100000 -c 1000 http://127.0.0.1:9618/
```

## 4.1. :computer:机器1

机器参数：
|          | 参数           |
|:--------:|:--------------:|
| 操作系统 | manjaro 20.0.4 |
| CPU      | i5-4210u       |
| 内存     | 8GB 2400MHz    |

### 4.1.1. :link:短连接
| 服务器名称  | t=1 | t=2 | t=3 | t=4 | t=5 | t=6 | t=7 | t=8 |
|:-----------:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| `Muduo `    |     |     |     |     |     |     |     |     |
| `WebServer` |     |     |     |     |     |     |     |     |
| `Zaver `    |     |     |     |     |     |     |     |     |
| `TKeed `    |     |     |     |     |     |     |     |     |
| `Shoot `    |     |     |     |     |     |     |     |     |
:bell:`t`为线程数量

### 4.1.2. :link:长连接

| 服务器名称  | t=1 | t=2 | t=3 | t=4 | t=5 | t=6 | t=7 | t=8 |
|:-----------:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| `Muduo `    |     |     |     |     |     |     |     |     |
| `WebServer` |     |     |     |     |     |     |     |     |
| `Zaver `    |     |     |     |     |     |     |     |     |
| `TKeed `    |     |     |     |     |     |     |     |     |
| `Shoot `    |     |     |     |     |     |     |     |     |

## 4.2. :computer:机器2

| |参数 |
|:-:|:-:|
|操作系统 |Centos 7 |
|CPU | |
|内存 |320GB 2666MHz |

<++>

### 4.2.1. :link:短连接
| 服务器名称  | t=1 | t=2 | t=3 | t=4   | t=5 | t=6 | t=7 | t=8 |
|:-----------:|:---:|:---:|:---:|:-----:|:---:|:---:|:---:|:---:|
| `Muduo `    |     |     |     |       |     |     |     |     |
| `WebServer` |     |     |     |       |     |     |     |     |
| `Zaver `    |     |     |     |       |     |     |     |     |
| `TKeed `    |     |     |     |       |     |     |     |     |
| `Shoot `    |     |     |     | 50576 |     |     |     |     |

### 4.2.2. :link:长连接
| 服务器名称  | t=1 | t=2 | t=3 | t=4 | t=5 | t=6 | t=7 | t=8 |
|:-----------:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| `Muduo `    |     |     |     |     |     |     |     |     |
| `WebServer` |     |     |     |     |     |     |     |     |
| `Zaver `    |     |     |     |     |     |     |     |     |
| `TKeed `    |     |     |     |     |     |     |     |     |
| `Shoot `    |     |     |     |     |     |     |     |     |



# 5. :hourglass:TODO

- 内存池，新建一个连接，需要新建一个`Connection`对象，建立一个内存池，在需要的时候直接从内存池中分配内存

# 6. :books:参考

- [Muduo](https://github.com/chenshuo/muduo)
- [WebServer](https://github.com/linyacool/WebServer)
- [Zaver](https://github.com/zyearn/zaver)
- [TKeed](https://github.com/linw7/TKeed)
- [handy](https://github.com/yedf/handy)
