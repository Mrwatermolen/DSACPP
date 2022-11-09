#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP
#include "MyList.hpp"

template <typename T> class MyQueue : public MyList<T> {
private:
public:
  MyQueue(){};
  ~MyQueue(){};

  void Enqueue(const T &e);
  T Dequeue();
  T &Front();
};

template <typename T> void MyQueue<T>::Enqueue(const T &e) {
  this->InsertAsLast(e);
}

template <typename T> T MyQueue<T>::Dequeue() {
  return this->Remove(this->First());
}

template <typename T> T &MyQueue<T>::Front() { return this->First()->data; }
#endif