#ifndef MYLIST_HPP
#define MYLIST_HPP
#include "MyListNode.hpp"

template <typename T>
class MyList {
 private:
  int _size;       // 规模
  Posi<T> header;  // 头
  Posi<T> tailer;  // 尾

 protected:
  void init();
  void clear();
  void copyForm(const MyList<T>& l);

 public:
  MyList();
  ~MyList();

  MyList<T>& operator=(const MyList<T>& l);

  int Size();
  bool Empty();
  Posi<T> First();
  Posi<T> Last();

  Posi<T> InsertBefore(Posi<T>, const T&);
  Posi<T> InsertAfter(Posi<T>, const T&);
  Posi<T> InsertAsFirst(const T&);
  Posi<T> InsertAsLast(const T&);
  T Remove(Posi<T> p);
  Posi<T> Find(const T&, int, Posi<T>);
  Posi<T> Search(const T&, int, Posi<T>);
  template <typename VST>
  void Traverse(VST&);
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

/// @brief 初始化时统一调用
/// @tparam T
template <typename T>
void MyList<T>::init() {
  _size = 0;
  header = new MyListNode<T>;
  tailer = new MyListNode<T>;
  header->pred = nullptr, header->succ = tailer;
  tailer->pred = header, tailer->succ = nullptr;
}

/// @brief 清空对外可见节点
/// @tparam T
template <typename T>
void MyList<T>::clear() {
  while (_size != 0) {
    Remove(header->succ);
    _size--;
  }
}

template <typename T>
void MyList<T>::copyForm(const MyList<T>& l) {
  Posi<T> p = l.header->succ;
  Posi<T> q = header;
  while (p && p != l.tailer) {
    q = InsertAfter(q, p->data);
    p = p->succ;
  }
}

template <typename T>
MyList<T>& MyList<T>::operator=(const MyList<T>& l) {
  if (this == &l) {
    return *this;
  }
  clear();
  copyForm(l);
}

/// @brief
/// @tparam T
/// @return 返回列表规模
template <typename T>
int MyList<T>::Size() {
  return _size;
}

/// @brief 列表是否为空
/// @tparam T
/// @return 为空返回true
template <typename T>
bool MyList<T>::Empty() {
  return _size == 0;
}

/// @brief
/// @tparam T
/// @return 返回第一个元素 若列表为空返回nullptr
template <typename T>
Posi<T> MyList<T>::First() {
  if (_size == 0) {
    return nullptr;
  }
  return header->succ;
}

/// @brief
/// @tparam T
/// @return 返回最后元素 若列表为空返回nullptr
template <typename T>
Posi<T> MyList<T>::Last() {
  if (_size == 0) {
    return nullptr;
  }
  return tailer->pred;
}

/// @brief 将e作为p的前驱插入
/// @tparam T
/// @param p 插入的节点
/// @param e 待插入元素
/// @return 元素e所在节点
template <typename T>
Posi<T> MyList<T>::InsertBefore(Posi<T> p, const T& e) {
  _size++;
  return p->InsertAsPred(e);
}

/// @brief 将e插入为尾部部
/// @tparam T
/// @param e 待插入元素
/// @return 元素e所在节点
template <typename T>
Posi<T> MyList<T>::InsertAfter(Posi<T> p, const T& e) {
  _size++;
  return p->InsertAsSucc(e);
}

/// @brief 将e插入为首部
/// @tparam T
/// @param e 待插入元素
/// @return 元素e所在节点
template <typename T>
Posi<T> MyList<T>::InsertAsFirst(const T& e) {
  _size++;
  return header->InsertAsSucc(e);
}

/// @brief 将e插入为末尾
/// @tparam T
/// @param e 待插入元素
/// @return 元素e所在节点
template <typename T>
Posi<T> MyList<T>::InsertAsLast(const T& e) {
  _size++;
  return tailer->InsertAsPred(e);
}

/// @brief 从列表中移除节点p
/// @tparam T
/// @param p
/// @return 节点p的数据
template <typename T>
T MyList<T>::Remove(Posi<T> p) {
  T e = p->data;
  p->pred->succ = p->succ;
  p->succ->pred = p->pred;
  delete p;
  _size--;
  return e;
}

/// @brief 在p的n个前驱中找到值为e的最靠后的元素
/// @tparam T
/// @param e
/// @param n
/// @param p
/// @return 返回寻找到的节点 无则返回nullptr
template <typename T>
Posi<T> MyList<T>::Find(const T& e, int n, Posi<T> p) {
  while (0 < n--) {
    if (e == p->pred->data) {
      return p->pred->data;
    }
    p = p->pred;
  }
  return nullptr;
}

/// @brief 遍历
/// @tparam T
/// @param visit 遍历器
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