/*
  $ g++ test_aqueue.cpp -L $PWD -l aqueue++ -Wl,-rpath,$PWD -o test_aqueue && ./test_aqueue
*/

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include "aqueue.hpp"

static unsigned processed_count;

void func_cb(const int *v)
{
  printf("\nThis is job %d", *v);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  printf("\nJob %d exit", *v);
  processed_count++;
}

int main()
{
  Queue queue;
  srand(0);
  std::vector<int> V;

  for (unsigned i = 0; i < 1000; i++)
  {
    int v = rand() % 100;
    V.emplace_back(std::move(v));
  }

  for (std::vector<int>::iterator v = V.begin(); v < V.end(); v++)
  {
    queue.enqueue([v]() { func_cb(&*v); });
    printf("Data %d...job added\n", *v);
  }

  while (queue.count_queue())
  {
    printf("\r%ld job(s) remaining", queue.count_queue());
  }

  printf("\n%d job(s) processed\n", processed_count);
  return 0;
}
