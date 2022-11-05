#ifndef MYLIST_HPP
#define MYLIST_HPP
#include "MyListNode.hpp"

template <typename T>
class MyList {
 private:
  int _size;
  Posi(T) header;
  Posi(T) tailer;

 protected:
  void init();
  void clear();
  void copyForm(const MyList<T>& l);

 public:
  MyList(/* args */);
  ~MyList();

  MyList<T>& operator=(const MyList<T>& l);

  int Size();
  bool Empty();
  Posi(T) First();
  Posi(T) Last();

  Posi(T) InsertBefore(Posi(T) p, const T& e);
  Posi(T) InsertAfter(Posi(T) p, const T& e);
  Posi(T) InsertAsFirst(const T& e);
  Posi(T) InsertAsLast(const T& e);
  T Remove(Posi(T) p);
  Posi(T) Find(const T& e, int n, Posi(T) p);
  Posi(T) Search(const T& e, int n, Posi(T) p);
  template <typename VST>
  void Traverse(VST& visit);
};

template <typename T>
MyList<T>::MyList() {
  init();
}

template <typename T>
MyList<T>::~MyList() {
  clear();
  delete header, tailer;
  header = nullptr, tailer = nullptr;
}

//
// 初始化时统一调用
//
template <typename T>
void MyList<T>::init() {
  _size = 0;
  header = new MyListNode<T>;
  tailer = new MyListNode<T>;
  header->pred = nullptr, header->succ = tailer;
  tailer->pred = header, tailer->succ = nullptr;
}

//
// 清空对外可见节点
//
template <typename T>
void MyList<T>::clear() {
  while (_size != 0) {
    Remove(header->succ);
    _size--;
  }
}

template <typename T>
void MyList<T>::copyForm(const MyList<T>& l) {
  Posi(T) p = l.header->succ;
  Posi(T) q = header;
  while (p && p != l.tailer) {
    q = InsertAfter(q, p->data);
    p = p->succ;
  }
}

//
// 重载=
//
template <typename T>
MyList<T>& MyList<T>::operator=(const MyList<T>& l) {
  if (this == &l) {
    return *this;
  }
  clear();
  copyForm(l);
}

//
// 返回列表规模
//
template <typename T>
int MyList<T>::Size() {
  return _size;
}

//
// 列表是否为空
//
template <typename T>
bool MyList<T>::Empty() {
  return _size == 0;
}

//
// 返回第一个元素 若列表为空返回nullptr
//
template <typename T>
Posi(T) MyList<T>::First() {
  if (_size == 0) {
    return nullptr;
  }
  return header->succ;
}

//
// 返回最后元素 若列表为空返回nullptr
//
template <typename T>
Posi(T) MyList<T>::Last() {
  if (_size == 0) {
    return nullptr;
  }
  return tailer->pred;
}

//
// 将e当做p的前驱插入
//
template <typename T>
Posi(T) MyList<T>::InsertBefore(Posi(T) p, const T& e) {
  _size++;
  return p->InsertAsPred(e);
}

//
// 将e当做p的后继插入
//
template <typename T>
Posi(T) MyList<T>::InsertAfter(Posi(T) p, const T& e) {
  _size++;
  return p->InsertAsSucc(e);
}

//
// 将e插入为首部
//
template <typename T>
Posi(T) MyList<T>::InsertAsFirst(const T& e) {
  _size++;
  return header->InsertAsSucc(e);
}

//
// 移除p
//
template <typename T>
T MyList<T>::Remove(Posi(T) p) {
  T e = p->data;
  p->pred->succ = p->succ;
  p->succ->pred = p->pred;
  delete p;
  _size--;
  return e;
}

//
// 在p的n个前驱中找到值为e的最靠后的元素 无则返回nullptr
//
template <typename T>
Posi(T) MyList<T>::Find(const T& e, int n, Posi(T) p) {
  while (0 < n--) {
    if (e == p->pred->data) {
      return p->pred->data;
    }
    p = p->pred;
  }
  return nullptr;
}

//
// 将e插入为末尾
//
template <typename T>
Posi(T) MyList<T>::InsertAsLast(const T& e) {
  _size++;
  return tailer->InsertAsPred(e);
}

template <typename T>
template <typename VST>
void MyList<T>::Traverse(VST& visit) {
  auto p = header->succ;
  while (p != tailer) {
    visit(p->data);
    p = p->succ;
  }
}

#endif