### 1.介绍

C++线程是C++11标准新加入的库功能（#include <thread>），C++之前一直又系统级API提供并发功能，此标准库可以让两个不同的线程使用两个不同的对象。

- 支持线程安全
- 支持类和函数并发运算
- 分离关注点
- 优化性能

### 2.并发和并发

并发：单核切换任务运行

并行：多核多线程运行

### 3.并发执行

- 简单并发

```c++
thread t(函数);
t.join()
```

其中join为等待子线程执行完毕的函数

- 带参数的并发

```c++
void update_data_for_widget(widget_id w,widget_data& data);
std::thread t(update_data_for_widget,w,data);
```

### 4.共享数据

当两个线程同时访问一个内存或者说是一个变量时有可能引起冲突，导致破坏数据。

- 互斥量，简单来说就是锁住一块代码，导致一个变量同一时候只能让一个线程访问（#include <mutex>)

```c++
std::mutex m;
m.lock();
some_list.push_back(new_value);
m.unlock();
```

- lock_guard特殊对象，可以在定义域结束后自动解锁

```c++
std::mutex some_mutex;
void proc1(int a)
{
    lock_guard<mutex> g1(some_mutex);
	some_list.push_back(new_value);
}//自动解锁
```

- unique_lock,lock_guard的加强版，可以使用lock、unlock、try_to_lock和defer_lock
  - defer_lock：始化了一个没有加锁的mutex
  - try_to_lock：尝试去锁定，**得保证锁处于unlock的状态**,然后尝试现在能不能获得锁；尝试用mutex的lock()去锁定这个mutex，但如果没有锁定成功，会立即返回，不会阻塞在那里，并继续往下执行

```c++
std::mutex some_mutex;
void proc1(int a)
{
    unique_lock<mutex> g1(some_mutex, defer_lock);
    some_list.push_back(new_value);
    g1.lock();
    some_list.push_back(new_value);
    g1.unlock();
    some_list.push_back(new_value);
    g1.lock();
    some_list.push_back(new_value);
}//自动解锁
```

### 5.condition_variable

用来唤醒线程，需要库函数\#include<condition_variable>

- wait(locker) ：阻塞线程，让线程等待执行
- notify_one(): 随机唤醒一个等待的线程
- notify_all(): 唤醒所有等待的线程

### 6.异步线程

需要#include<future>，有两个主要方法std::async和std::future

- async，启动空的异步任务
- future，异步任务的对象，可以通过get函数拿到线程的执行返回值

```
 future<double> fu = async(t1, a, b);
 cout << "计算结果：" << fu.get() << endl;//阻塞并取得线程结果
```

