#ifndef MYPRIORITYQUEUE_HPP
#define MYPRIORITYQUEUE_HPP

#include "MyBinNode.hpp"
#include <algorithm>

template <typename T> struct MaxHeap {
  bool operator()(const T &a, const T &b) { return a < b; }
};

template <typename T> struct MinHeap {
  bool operator()(const T &a, const T &b) { return a > b; }
};

template <typename T, typename compare> class MyPriorityQueue {
protected:
  compare _compare;

public:
  virtual T GetMax() = 0;             // 获得优先级最高的词条
  virtual T DelMax() = 0;             // 删除优先级最高的词条
  virtual void Insert(const T &) = 0; // 按优先级顺序插入词条
  virtual bool Empty() = 0;
  MyPriorityQueue() : _compare(compare()){};
  virtual ~MyPriorityQueue(){};
};

#include "MyBinTree.hpp"
#include "MyVector.hpp"

template <typename T, typename compare>
class MyPQCompleteHeap : public MyPriorityQueue<T, compare>,
                         public MyVector<T> {
protected:
  Rank percolateUp(Rank);
  Rank percolateDown(Rank, Rank);
  void heapify(Rank);

public:
  MyPQCompleteHeap(const MyVector<T> &);
  // MyPQCompleteHeap(const T *);
  ~MyPQCompleteHeap(){};
  T GetMax() override;
  T DelMax() override;
  void Insert(const T &) override;
  bool Empty() override;
};

static inline Rank parent(Rank i) { return ((i - 1) >> 1); }
static inline Rank leftChild(Rank i) { return ((i << 1) + 1); }
static inline Rank rightChild(Rank i) { return ((i + 1) << 1); }
static inline Rank lastInteral(Rank n) { return ((n - 1) >> 1); }

template <typename T, typename compare>
Rank MyPQCompleteHeap<T, compare>::percolateUp(Rank i) {
  T e;
  e = this->_elem[i];
  while (0 < parent(i)) {
    Rank j = parent(i);
    /* if (e <= this->_elem[j]) {
      break;
    } */
    if (this->_compare(e, this->_elem[j])) {
    }
    this->_elem[i] = this->_elem[j];
    i = j;
  }
  this->_elem[i] = e;
  return i;
}

template <typename T, typename compare>
Rank MyPQCompleteHeap<T, compare>::percolateDown(Rank n, Rank i) {
  T e;
  e = this->_elem[i];
  while (leftChild(i) < n) {
    Rank j = leftChild(i);
    if (j + 1 < this->_size &&
        this->_compare(this->_elem[j], this->_elem[j + 1])) {
      ++j;
    }
    if (this->_compare(this->_elem[j], e)) {
      break;
    }
    this->_elem[i] = this->_elem[j];
    i = j;
  }
  this->_elem[i] = e;
  return i;
}

template <typename T, typename compare>
void MyPQCompleteHeap<T, compare>::heapify(Rank n) {
  for (Rank i = lastInteral(n); i >= 0; i--) {
    percolateDown(n, i);
  }
};

template <typename T, typename compare>
MyPQCompleteHeap<T, compare>::MyPQCompleteHeap(const MyVector<T> &v)
    : MyPriorityQueue<T, compare>(), MyVector<T>(v) {
  heapify(this->_size);
}

template <typename T, typename compare>
void MyPQCompleteHeap<T, compare>::Insert(const T &e) {
  MyVector<T>::Insert(this->_size, e);
  percolateUp(this->_size - 1);
}

template <typename T, typename compare>
T MyPQCompleteHeap<T, compare>::GetMax() {
  return this->_elem[0];
}

template <typename T, typename compare>
T MyPQCompleteHeap<T, compare>::DelMax() {
  T e;
  e = this->_elem[0];
  this->_elem[0] = this->_elem[this->_size - 1];
  this->RemoveAt(this->_size - 1);
  if (this->Empty()) {
    return e;
  }

  percolateDown(this->_size, 0);
  return e;
}

template <typename T, typename compare>
bool MyPQCompleteHeap<T, compare>::Empty() {
  return MyVector<T>::Empty();
}

template <typename T, typename compare>
class MyLeftHeap : public MyPriorityQueue<T, compare>, public MyBinTree<T> {

public:
  MyLeftHeap() : MyPriorityQueue<T, compare>() {}
  ~MyLeftHeap() {}
  T GetMax() override;
  T DelMax() override;
  void Insert(const T &) override;
  bool Empty() override;
};

template <typename T, typename compare>
static BinNodePosi<T> meger(BinNodePosi<T> a, BinNodePosi<T> b,
                            const compare &f) {
  if (!a) {
    return b;
  }
  if (!b) {
    return a;
  }
  if (f(a->data, b->data)) {
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

template <typename T, typename compare>
void MyLeftHeap<T, compare>::Insert(const T &e) {
  BinNodePosi<T> a = new MyBinNode<T>(e);
  a->nlp = 1;
  this->_root = meger(this->_root, a, this->_comparer);
  this->_root->parent = nullptr;
}

template <typename T, typename compare> T MyLeftHeap<T, compare>::DelMax() {
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
  this->_root = meger(l, r, this->_comparer);
  if (this->_root) {
    this->_root->parent = nullptr;
  }
  return e;
}

template <typename T, typename compare> T MyLeftHeap<T, compare>::GetMax() {
  return this->_root->data;
}

template <typename T, typename compare> bool MyLeftHeap<T, compare>::Empty() {
  return this->_root == nullptr;
}

#endif