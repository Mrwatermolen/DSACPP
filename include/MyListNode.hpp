#ifndef MYLISTNODE_HPP
#define MYLISTNODE_HPP

template <typename T> struct MyListNode;
template <typename T> using Posi = MyListNode<T> *;

template <typename T> struct MyListNode {
  T data;
  Posi<T> pred; // 前驱
  Posi<T> succ; // 后继
  MyListNode();
  MyListNode(const T &e, Posi<T> p = nullptr, Posi<T> s = nullptr);

  MyListNode<T> &operator=(const MyListNode<T> &);

  Posi<T> InsertAsPred(const T &); // 前插
  Posi<T> InsertAsSucc(const T &); // 后插
};

template <typename T>
MyListNode<T>::MyListNode() : pred(nullptr), succ(nullptr) {}

/// @brief
/// @tparam T
/// @param e 元素3
/// @param p 前驱
/// @param s 后继
template <typename T>
MyListNode<T>::MyListNode(const T &e, Posi<T> p, Posi<T> s)
    : data(e), pred(p), succ(s) {}

template <typename T>
MyListNode<T> &MyListNode<T>::operator=(const MyListNode<T> &n) {
  if (this == &n) {
    return *this;
  }
  if (!n) {
    pred = nullptr;
    succ = nullptr;
    return this;
  }
  pred = n->pred;
  succ = n->succ;
  data = n->data;
  return this;
}

/// @brief 将元素e作为节点的前驱
/// @tparam T
/// @param e
/// @return 元素e所在节点
template <typename T> Posi<T> MyListNode<T>::InsertAsPred(const T &e) {
  Posi<T> q = new MyListNode<T>(e, pred, this);
  pred->succ = q;
  pred = q;
  return q;
}

/// @brief 将元素e作为节点的后继
/// @tparam T
/// @param e
/// @return 元素e所在节点
template <typename T> Posi<T> MyListNode<T>::InsertAsSucc(const T &e) {
  Posi<T> q = new MyListNode<T>(e, this, succ);
  succ->pred = q;
  succ = q;
  return q;
}

#endif