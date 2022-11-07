#ifndef MYSPLAY_HPP
#define MYSPLAY_HPP
#include "MyBST.hpp"

/// @brief 二叉搜索伸展树
template <typename T>
class MySlpay : public MyBST<T> {
 private:
 protected:
  BinNodePosi<T> splay(BinNodePosi<T>);

 public:
  MySlpay(const T& r);
  ~MySlpay();

  BinNodePosi<T>& Search(const T&);
  BinNodePosi<T> Insert(const T&);
  bool Remove(const T&);
};

template <typename T>
MySlpay<T>::MySlpay(const T& r) : MyBST<T>(r) {}

template <typename T>
MySlpay<T>::~MySlpay() {}

/// @brief 伸展操作 将节点v伸展至树根
/// @tparam T
/// @param v 要伸展的节点
/// @return 伸展至树根的节点
template <typename T>
BinNodePosi<T> MySlpay<T>::splay(BinNodePosi<T> v) {
  BinNodePosi<T> p = nullptr, g = nullptr;
  while (true) {
    p = v->parent;
    if (!p) {
      break;
    }
    g = p->parent;
    if (!g) {
      break;
    }
    if (v == p->lChild) {
      // zig
      if (p == g->lChild) {
        // zig
        // 子孙同侧时 不同于AVL 先对祖父节点进行旋转
        g->Zig();
        p->Zig();
      } else {
        // zag
        p->Zig();
        g->Zag();
      }
    } else {
      // zag
      if (p == g->rChild) {
        // zag
        g->Zag();
        p->Zag();
      } else {
        // zig
        p->Zag();
        g->Zig();
      }
    }
    this->updateHeight(g);
    this->updateHeight(p);
    this->updateHeight(v);
  }
  if ((p = v->parent)) {
    if (v == p->lChild) {
      p->Zig();
    } else {
      p->Zag();
    }
  }
  // this->_root = v;
  return v;
}

/// @brief 查找e 集成伸展操作
/// @param e 要查询的关键码
/// @return 成功返回指向关键码为e且真实存在的节点的引用 失败时
/// 指向最后一次试图转向的空节点
template <typename T>
BinNodePosi<T>& MySlpay<T>::Search(const T& e) {
  BinNodePosi<T>& x = this->searchIn(this->_root, e, this->_hot);
  if (x) {
    this->_root = splay(x);
  } else {
    this->_root = splay(this->_hot);
  }
  return this->_root;
}

/// @brief 将关键字e插入树中 集成伸展操作 插入的节点会伸展到树根
/// @tparam T
/// @param e
/// @return 树根
template <typename T>
BinNodePosi<T> MySlpay<T>::Insert(const T& e) {
  if (!this->_root) {
    this->_root = new MyBinNode<T>(e);
    this->_size++;
    return this->_root;
  }
  BinNodePosi<T> T0 = nullptr, T1 = nullptr, x = Search(e);
  if (x->data == e) {
    return x;
  }
  if (x->data < e) {
    T0 = x;
    T1 = x->rChild;
    x->rChild = nullptr;  // 分裂完成
    x = new MyBinNode<T>(e);
    x->lChild = T0;
    x->rChild = T1;
    T0->parent = x;
    if (T1) {
      T1->parent = x;
    }
  } else {
    T0 = x->lChild;
    T1 = x;
    x->lChild = nullptr;
    x = new MyBinNode<T>(e);
    x->lChild = T0;
    x->rChild = T1;
    T1->parent = x;
    if (T0) {
      T0->parent = x;
    }
  }
  this->_size++;
  this->_root = x;
  return this->_root;
}

/// @brief 将关键字e从树中移除 集成伸展操作
/// @tparam T
/// @param e
/// @return 是否成功
template <typename T>
bool MySlpay<T>::Remove(const T& e) {
  BinNodePosi<T> T0 = nullptr, T1 = nullptr, x = Search(e);
  if (this->_root->data != e) {
    return false;
  }
  BinNodePosi<T> succ = this->_root->Succ();
  T0 = x->lChild, T1 = x->rChild;
  this->_size--;
  // release(x);
  if (!succ) {
    this->_root = T0;
    if (T0) {
      T0->parent = nullptr;
    }
    return true;
  }

  this->_root = T1;
  T1->parent = nullptr;
  succ->lChild = T0;
  if (T0) {
    T0->parent = succ;
  }
  return true;
}

#endif