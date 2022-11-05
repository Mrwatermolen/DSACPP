#ifndef MYSTACK_HPP
#define MYSTACK_HPP
#include "MyVector.hpp"

template <typename T>
class MyStack : public MyVector<T> {
 private:
 public:
  MyStack();
  ~MyStack();

  void Push(const T& e);
  T Pop();
  T& Top();
};

template <typename T>
MyStack<T>::MyStack() {}

template <typename T>
MyStack<T>::~MyStack() {}

template <typename T>
void MyStack<T>::Push(const T& e) {
  this->Insert(this->Size(), e);
}

template <typename T>
T MyStack<T>::Pop() {
  return this->RemoveAt(this->Size() - 1);
}

template <typename T>
T& MyStack<T>::Top() {
  return (*this)[this->Size() - 1];
}

#endif