
> 线程：一个正在运行的函数

## 1. 线程的概念
- posix线程是一种标准，而不是实现
- 线程标识：`pthread_t`，posix类型的线程
- `pthread_equal()`比较两个线程是否相等
- `pthread_self() `返回当前线程标识
## 2. 线程的创建
    ```c
    int pthread_create(pthread_t* thread,const pthread_attr_t* attr,void*(*start_routine)(void*),void* arg)
    ```
   ***线程的调度取决于调度策略***
    
   ### 线程终止有三种方式
   - 线程从启动例程中返回，返回值就是线程的退出码
   - 线程可以被同一进程中的其他线程取消
   - 线程调用`pthread_exit()`函数
   ```c
   void pthread_exit(void* retval);
   //其中retval是该函数的返回值
   ```
   **pthread_join()**
   ```c
   int pthread_join(pthread_t,void ** retval);
   //if retval == NULL 只收尸，不关心线程的状态
   //else 给一个指针变量的地址，作为返回值
   ```




   ### 栈清理

   :bell:需要注意的是：下面两个函数都是宏定义，且必须成对出现
   `pthread_cleanup_push()`
   `pthread_clearnup_pop()`

   ```c
   void pthread_cleanup_push(void (*routine)(void* argv))
   void pthread_cleanup_pop(int execute)
   //execute 0 则不执行
   //execute 1 则执行钩子函数
   ```
   ### 线程的取消

   `pthread_cancel()`
   ```c
   int pthread_cancel(pthread_t)
   ```
   :bell:线程的取消可能会存在问题,比如下面这个例子，fd1还没有关闭的情况就被取消了，还没来得及关闭。可以通过钩子函解决这个问题

   线程的取消有两种状态：
   - 允许
     - 异步
     - 推迟->推迟到cancel点再取消
        > poxis 定义的cancel 点：都是引发可能引起阻塞的系统调用
   - 不允许
   ```c
   fd1 = open();
   if (fd1 < 0) {
      perror();
      exit(1);
   }
    ----> cleanup_push(fd1);
   fd2 = open();
   if (fd2 < 0) {
      perror();
      exit(1);
   }
    ----> cleanup_push(fd2);
   ```

   `pthread_setcancelstate()`
   设置是否允许取消   
   
   `pthread_setcanceltype()`
   设置取消方式

   `pthread_testcancel()`
   本函数什么都不做，就是一个取消点

   ### 线程分离

   `pthread_detach`



## 3. 线程的同步

   ### 互斥量
   > 限制一段代码以独占的方式运行

   `pthread_mutex_t`


   `pthread_mutex_init(pthread_mutex_t,const pthread_mutexattr)`


   `pthread_mutex_destroy()`

   
   `pthread_mutex_lock()`

   
   `pthread_mutex_trylock()`

   
   `pthread_mutex_unlock()`

   ***注意临界区中所有的跳转语句***

   > 在临界区内跳转不需解锁，但是一旦跳到临界区外，那必须解锁

   ### 条件变量:star:

   `pthread_cond_t`
   

   ### 信号量

   可以通过条件变量和互斥量实现信号量的功能。

   ### 读写锁
   > 相当于互斥加共享锁，读锁->共享锁，写锁->互斥锁

   TODO:共享锁
   补充：共享锁


## 4. 线程的属性
   `pthread_attr_init(pthread_attr_t* attr)`

   线程同步的属性
## 5. 重入

   TODO:explain 
   线程和信号
   线程与fork
