#ifndef MYAVL_HPP
#define MYAVL_HPP
#include "MyBST.hpp"
// 理想平衡状态
#define Balanced(x) (stature((x)->lChild) == stature((x)->rChild))
// 平衡因子
#define Balfac(x) (stature((x)->lChild) - stature((x)->rChild))
// 是否满足AVL树的平衡条件
#define AvlBalanced(x) ((-2 < Balfac(x)) && (Balfac(x) < 2))

/// @brief 平衡二叉搜索树
template <typename T> class MyAVL : public MyBST<T> {
private:
protected:
  int updateHeight(BinNodePosi<T>);

public:
  MyAVL(const T &r) : MyBST<T>(r) {}
  ~MyAVL() {}

  BinNodePosi<T> Insert(const T &);
  bool Remove(const T &);
};

template <typename T> int MyAVL<T>::updateHeight(BinNodePosi<T> x) {
  int c = -1;
  if (x->lChild) {
    c = max(c, x->lChild->height);
  }
  if (x->rChild) {
    c = max(c, x->rChild->height);
  }
  x->height = c + 1;
  return x->height;
}

/// @brief 将关键字插入AVL树中
/// @tparam T
/// @param e
/// @return 插入的节点
template <typename T> BinNodePosi<T> MyAVL<T>::Insert(const T &e) {
  BinNodePosi<T> &x = this->Search(e);
  if (x) {
    // 防止雷同元素
    return x;
  }
  x = new MyBinNode<T>(e, this->_hot);
  this->_size++;
  BinNodePosi<T> p = x; // 复制x的值
  // 检查平衡
  for (BinNodePosi<T> i = p->parent; i; i = i->parent) {
    if (!AvlBalanced(i)) {
      // 定位
      BinNodePosi<T> v = nullptr;
      if (stature(i->lChild) > stature(i->rChild)) {
        if (stature(i->lChild->lChild) > stature(i->lChild->rChild)) {
          v = i->lChild->lChild;
        } else {
          v = i->lChild->rChild;
        }
      } else {
        if (stature(i->rChild->rChild) > stature(i->rChild->lChild)) {
          v = i->rChild->rChild;
        } else {
          v = i->rChild->lChild;
        }
      }
      v = this->rotateAt(v);
      // 反向连接
      if (v->parent) {
        if (v->parent->lChild == i) {
          v->parent->lChild = v;
        } else {
          v->parent->rChild = v;
        }
      } else {
        this->_root = v;
      }
      break;
    } else {
      updateHeight(i);
    }
  }

  return p;
}

/// @brief 删除关键码为e的节点
/// @tparam T
/// @param e
/// @return 是否删除成功
template <typename T> bool MyAVL<T>::Remove(const T &e) {
  BinNodePosi<T> &x = this->Search(e);
  if (!x) {
    return false;
  }
  this->removeAt(x, this->_hot);
  // 检查平衡
  for (BinNodePosi<T> i = this->_hot->parent; i; i = i->parent) {
    if (!AvlBalanced(i)) {
      // 定位
      BinNodePosi<T> v = nullptr;
      if (stature(i->lChild) > stature(i->rChild)) {
        if (stature(i->lChild->lChild) > stature(i->lChild->rChild)) {
          v = i->lChild->lChild;
        } else {
          v = i->lChild->rChild;
        }
      } else {
        if (stature(i->rChild->rChild) > stature(i->rChild->lChild)) {
          v = i->rChild->rChild;
        } else {
          v = i->rChild->lChild;
        }
      }
      v = this->rotateAt(v);
      // 反向连接
      if (v->parent) {
        if (v->parent->lChild == i) {
          v->parent->lChild = v;
        } else {
          v->parent->rChild = v;
        }
      } else {
        this->_root = v;
      }
    }

    updateHeight(i);
  }
  return true;
}

#endif