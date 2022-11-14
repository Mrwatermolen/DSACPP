#ifndef MYPRIORITYQUEUE_HPP
#define MYPRIORITYQUEUE_HPP

#include "MyBinNode.hpp"
#include <algorithm>
#include <cstddef>
#include <type_traits>
template <typename T> class MyPriorityQueue {
public:
  virtual T GetMax() = 0;             // 获得优先级最高的词条
  virtual T DelMax() = 0;             // 删除优先级最高的词条
  virtual void Insert(const T &) = 0; // 按优先级顺序插入词条
};

#include "MyBinTree.hpp"
#include "MyVector.hpp"

template <typename T>
class MyPQCompleteHeap : public MyPriorityQueue<T>, public MyVector<T> {
protected:
  Rank percolateUp(Rank);
  Rank percolateDown(Rank, Rank);
  void heapify(Rank);

public:
  MyPQCompleteHeap(const MyVector<T> &);
  MyPQCompleteHeap(const T *);
  // MyPQCompleteHeap();
  T GetMax() override;
  T DelMax() override;
  void Insert(const T &) override;
};

static inline Rank parent(Rank i) { return ((i - 1) >> 1); }
static inline Rank leftChild(Rank i) { return ((i << 1) + 1); }
static inline Rank rightChild(Rank i) { return ((i + 1) << 1); }
static inline Rank lastInteral(Rank n) { return ((n - 1) >> 1); }

template <typename T> Rank MyPQCompleteHeap<T>::percolateUp(Rank i) {
  T e = this->_elem[i];
  while (0 < parent(i)) {
    Rank j = parent(i);
    if (e <= this->_elem[j]) {
      break;
    }
    this->_elem[i] = this->_elem[j];
    i = j;
  }
  this->_elem[i] = e;
  return i;
}

template <typename T> Rank MyPQCompleteHeap<T>::percolateDown(Rank n, Rank i) {
  T e = this->_elem[i];
  while (leftChild(i) < n) {
    Rank j = leftChild(i);
    if (j + 1 < this->_size && this->_elem[j] < this->_elem[j + 1]) {
      ++j;
    }
    if (this->_elem[j] <= e) {
      break;
    }
    this->_elem[i] = this->_elem[j];
    i = j;
  }
  this->_elem[i] = e;
  return i;
}

template <typename T> void MyPQCompleteHeap<T>::heapify(Rank n) {
  for (Rank i = lastInteral(n); i >= 0; i--) {
    percolateDown(n, i);
  }
};

template <typename T>
MyPQCompleteHeap<T>::MyPQCompleteHeap(const MyVector<T> &v)
    : MyPriorityQueue<T>(), MyVector<T>(v) {
  heapify(this->_size);
}

template <typename T> void MyPQCompleteHeap<T>::Insert(const T &e) {
  MyVector<T>::Insert(this->_size, e);
  percolateUp(this->_size - 1);
}

template <typename T> T MyPQCompleteHeap<T>::GetMax() { return this->_elem[0]; }

template <typename T> T MyPQCompleteHeap<T>::DelMax() {
  T e = this->_elem[0];
  this->_elem[0] = this->_elem[this->_size - 1];
  this->RemoveAt(this->_size - 1);
  if (this->Empty()) {
    return e;
  }

  percolateDown(this->_size, 0);
  return e;
}

template <typename T>
class MyLeftHeap : public MyPriorityQueue<T>, public MyBinTree<T> {
public:
  T GetMax() override;
  T DelMax() override;
  void Insert(const T &) override;
};

template <typename T>
static BinNodePosi<T> meger(BinNodePosi<T> a, BinNodePosi<T> b) {
  if (!a) {
    return b;
  }
  if (!b) {
    return a;
  }
  if (a->data < b->data) {
    swap(a, b);
  }
  a->rChild = merge(a->rChild, b);
  a->rChild->parent = a;
  if (!a->lChild || a->lChild->nlp < a->rChild->nlp) {
    swap(a->lChild, a->rChild);
  }
  a->nlp = a->rChild ? 1 + a->rChild->nlp : 1;
  return a;
}

template <typename T> void MyLeftHeap<T>::Insert(const T &e) {
  BinNodePosi<T> a = new MyBinNode<T>(e);
  a->nlp = 1;
  this->_root = meger(this->_root, a);
  this->_root->parent = nullptr;
}

template <typename T> T MyLeftHeap<T>::DelMax() {
  T e = this->_root->data;
  BinNodePosi<T> l = this->_root->lChild, r = this->_root->lChild;
  if (l) {
    l->parent = nullptr;
  }
  if (r) {
    r->parent = nullptr;
  }
  delete this->_root;
  this->_root = nullptr;
  this->_size--;
  this->_root = meger(l, r);
  if (this->_root) {
    this->_root->parent = nullptr;
  }
  return e;
}

template <typename T> T MyLeftHeap<T>::GetMax() { return this->_root->data; }

#endif