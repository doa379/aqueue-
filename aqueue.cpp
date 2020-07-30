#include <chrono>
#include "aqueue.hpp"

Queue::Queue(void)
{
  quit = 0;
  th = new std::thread(&Queue::worker, this);
}

Queue::~Queue(void)
{
  while (lock.test_and_set(std::memory_order_acquire))
    ;
  quit = 1;
  lock.clear(std::memory_order_release);
  th->join();
  delete th;
}

void Queue::clear(void)
{
  while (lock.test_and_set(std::memory_order_acquire))
    ;
  while (!q.empty()) q.pop();
  lock.clear(std::memory_order_release);
}
/*
void Queue::worker(void)
{  
  while (1)
  {
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
  while (lock.test_and_set(std::memory_order_acquire))
    ;
  q.push(job);
  lock.clear(std::memory_order_release);
}

size_t Queue::count(void)
{
  while (lock.test_and_set(std::memory_order_acquire))
    ;
  size_t size = q.size();
  lock.clear(std::memory_order_release);
  return size;
}
*/

void Queue::worker(void)
{  
  while (!quit)
  {
    if (q.size())
    {
      std::function<void()> curr_job = q.front();
      curr_job(); 
      q.pop();
    }

    else
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Queue::enqueue(std::function<void()> job)
{
  q.push(job);
}

size_t Queue::count(void)
{
  size_t size = q.size();
  return size;
}
