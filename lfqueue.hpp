#ifndef LFQUEUE_HPP
#define LFQUEUE_HPP

#include <thread>
#include <queue>
//#include <mutex>
//#include <condition_variable>
#include <functional>
#include <atomic>

class Queue
{
  std::thread *th;
  void worker(void);
  std::queue<std::function<void()>> q;
  //std::mutex q_mtx;
  //std::condition_variable cv;
  std::atomic_flag lock;
  bool quit;

public:
  Queue(void);
  ~Queue(void);
  void clear_queue(void);
  void enqueue(std::function<void()>);
  size_t count_queue(void);
};

#endif
