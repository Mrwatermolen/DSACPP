#include "testMyPQ.h"
#include "MyPriorityQueue.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

void testMyPQ() {
  auto f = [](int a) { std::cout << a << " "; };
  MyVector<int> a;
  srand(time(0));
  for (int i = 0; i < (rand() % 10) + 8; i++) {
    a.Insert(a.Size(), rand() % 20);
  }
  a.Traverse(f);
  MyPQCompleteHeap<int, MaxHeap<int>> mpq(a);
  std::cout << std::endl;
  mpq.Traverse(f);
  std::cout << "heap sort" << std::endl;
  heapSort(a);
  a.Traverse(f);
}

void heapSort(MyVector<int> &v) {
  MyPQCompleteHeap<int, MinHeap<int>> h(v);
  for (int i = 0; i < v.Size(); i++) {
    v[i] = h.DelMax();
  }
}