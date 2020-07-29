/*
  $ g++ test_lfqueue.cpp -L $PWD -l lfqueue++ -Wl,-rpath,$PWD -o test_lfqueue && ./test_lfqueue
*/

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "lfqueue.hpp"

void func_cb(const int *v)
{
  printf("\nThis is job %d", *v);
  sleep(1);
  printf("\nJob %d exit", *v);
}

int main()
{
  Queue queue;
  srand(0);
  std::vector<int> V;

  for (unsigned i = 0; i < 100; i++)
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
    sleep(1);
  }

  return 0;
}
