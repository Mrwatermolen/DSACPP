#ifndef MYLISTNODE_HPP
#define MYLISTNODE_HPP
#define Posi(T) MyListNode<T>*

template <typename T>
struct MyListNode {
  T data;
  Posi(T) pred;  // 前驱
  Posi(T) succ;  // 后继
  MyListNode(){};
  MyListNode(const T& e, Posi(T) p = nullptr, Posi(T) s = nullptr)
      : data(e), pred(p), succ(s) {}
  Posi(T) InsertAsPred(const T& e);  // 前插
  Posi(T) InsertAsSucc(const T& e);  // 后插
};

template <typename T>
Posi(T) MyListNode<T>::InsertAsPred(const T& e) {
  Posi(T) q = new MyListNode<T>(e, pred, this);
  pred->succ = q;
  pred = q;
  return q;
}

template <typename T>
Posi(T) MyListNode<T>::InsertAsSucc(const T& e) {
  Posi(T) q = new MyListNode<T>(e, this, succ);
  succ->pred = q;
  succ = q;
  return q;
}

#endif