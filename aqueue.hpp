#ifndef AQUEUE_HPP
#define AQUEUE_HPP

#include <thread>
#include <queue>
#include <functional>
#include <atomic>

class Queue
{
  std::thread *th;
  void worker(void);
  std::queue<std::function<void()>> q;
  std::atomic_flag lock = ATOMIC_FLAG_INIT;
  bool quit;

public:
  Queue(void);
  ~Queue(void);
  void clear(void);
  void enqueue(std::function<void()>);
  size_t count(void);
};

#endif
