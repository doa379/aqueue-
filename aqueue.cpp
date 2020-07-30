#include "aqueue.hpp"

Queue::Queue(void)
{
  quit = 0;
  //lock.atomic_flag_clear_explicit(memory_order_relaxed);
  //lock.clear(std::memory_order_release);
  th = new std::thread(&Queue::worker, this);
}

Queue::~Queue(void)
{
  /*
  {
    std::lock_guard<std::mutex> lock(q_mtx);
    quit = 1;
  }
  cv.notify_one();
  */
  while (lock.test_and_set(std::memory_order_acquire))
    ;
  quit = 1;
  lock.clear(std::memory_order_release);
  th->join();
  delete th;
}

void Queue::clear_queue(void)
{
  /*
  std::lock_guard<std::mutex> lock(q_mtx);
  while (!q.empty()) q.pop();
  cv.notify_one();
  */
}

void Queue::worker(void)
{  
  while (1)
  {
    /*
    {
      std::unique_lock<std::mutex> lock(q_mtx);
      cv.wait(lock, [this](){ return !q.empty() || quit; });

      if (quit)
        return;

      curr_job = q.front();
    }

    curr_job();

    {
      std::lock_guard<std::mutex> lock(q_mtx);
      q.pop();
    }
*/    
    while (lock.test_and_set(std::memory_order_acquire))
      ;
    
    if (quit)
    {
      lock.clear(std::memory_order_release);
      return;
    }

    if (q.size())
    {
      std::function<void()> curr_job = q.front();
      lock.clear(std::memory_order_release);
      curr_job(); 
      while (lock.test_and_set(std::memory_order_acquire))
        ;
      q.pop();
      lock.clear(std::memory_order_release);
    }

    else
      lock.clear(std::memory_order_release);
  }
}

void Queue::enqueue(std::function<void()> job)
{
  /*
  std::lock_guard<std::mutex> lock(q_mtx);
  q.push(job);
  cv.notify_one();
  */
  while (lock.test_and_set(std::memory_order_acquire))
    ;
  q.push(job);
  lock.clear(std::memory_order_release);
}

size_t Queue::count_queue(void)
{
  /*
  std::lock_guard<std::mutex> lock(q_mtx);
  return q.size();
  */
  while (lock.test_and_set(std::memory_order_acquire))
    ;
  size_t size = q.size();
  lock.clear(std::memory_order_release);
  return size;
}

