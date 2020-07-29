/*
  $ g++ test_lfqueue.cpp -L $PWD -l lfqueue++ -Wl,-rpath,$PWD -o test_lfqueue && ./test_lfqueue
*/

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "lfqueue.hpp"

void func_cb(const int *v)
{
  std::cout << *v << std::endl;
  sleep(5);
  std::cout << "Job exit" << std::endl;
}

int main()
{
  Queue queue;
  srand(0);
  std::vector<int> V;

  for (unsigned i = 0; i < 5; i++)
  {
    int v = rand() % 100;
    V.emplace_back(std::move(v));
  }

  for (std::vector<int>::iterator v = V.begin(); v < V.end(); v++)
  {
    queue.enqueue([v]() { func_cb(&*v); });
    std::cout << "Data " << *v << "job added" << std::endl;
  }

  while (queue.count_queue())
  {
    std::cout << queue.count_queue() << " jobs remaining" << std::endl;
    sleep(1);
  }

  return 0;
}
