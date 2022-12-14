#ifndef MYBINNODE_HPP
#define MYBINNODE_HPP
#include <algorithm>
#include <iostream>

template <typename T> struct MyBinNode;
template <typename T> using BinNodePosi = MyBinNode<T> *;
typedef enum { BLACK, RED } BinNColor;

/// @return 返回节点的高度
template <typename T> inline int stature(BinNodePosi<T> p) {
  return (p ? p->height : -1);
}

/// @brief 是否为黑节点 规定空节点也为黑节点
template <typename T> inline bool isBlack(BinNodePosi<T> p) {
  if (!p) {
    return true;
  }
  return p->color == BLACK;
}

template <typename T> struct MyBinNode {
  T data;
  BinNodePosi<T> lChild; // 左孩子
  BinNodePosi<T> rChild; // 右孩子
  BinNodePosi<T> parent; // 父亲
  int height;            // 高度
  BinNColor color;       // 颜色
  int nlp;

  int Size();
  BinNodePosi<T> InsertAsLC(const T &e);
  BinNodePosi<T> InsertAsRC(const T &e);
  BinNodePosi<T> Succ();

  MyBinNode(const T &e, BinNodePosi<T> p = nullptr, BinNodePosi<T> l = nullptr,
            BinNodePosi<T> r = nullptr, int h = 0, BinNColor c = BLACK,
            int n = 1)
      : data(e), parent(p), lChild(l), rChild(r), height(h), color(c), nlp(n){};
  MyBinNode()
      : parent(nullptr), lChild(nullptr), rChild(nullptr), height(0),
        color(BLACK), nlp(0){};

  BinNodePosi<T> Zig();
  BinNodePosi<T> Zag();
  inline bool IsLeaf() const;

  bool operator<(const MyBinNode &e) const { return this->data < e.data; }
  bool operator<=(const MyBinNode &e) const { return this->data <= e.data; }
  bool operator>(const MyBinNode &e) const { return this->data > e.data; }
  bool operator>=(const MyBinNode &e) const { return this->data >= e.data; }
  bool operator!=(const MyBinNode &e) const { return this->data != e.data; }
  bool operator==(const MyBinNode &e) const { return this->data != e.data; }
};

/// @brief
/// @tparam T
/// @param e
/// @return 以节点为根的子树的规模
template <typename T> int MyBinNode<T>::Size() {
  int s = 1;
  if (lChild) {
    s += lChild->Size();
  }
  if (rChild) {
    s += rChild->Size();
  }
  return s;
}

/// @brief 作为节点左孩子插入 要求节点的左孩子为空 不做检查
/// @tparam T
/// @param e
/// @return 节点的左孩子
template <typename T> BinNodePosi<T> MyBinNode<T>::InsertAsLC(const T &e) {
  return (lChild = new MyBinNode(e, this));
}

/// @brief 作为节点右孩子插入 要求节点的右孩子为空 不做检查
/// @tparam T
/// @param e
/// @return 节点的右孩子
template <typename T> BinNodePosi<T> MyBinNode<T>::InsertAsRC(const T &e) {
  return (rChild = new MyBinNode(e, this));
}

template <typename T>
/// @brief
/// @return 中序遍历下的节点的后继
BinNodePosi<T> MyBinNode<T>::Succ() {
  BinNodePosi<T> succ = nullptr;
  BinNodePosi<T> x = rChild;
  while (x) {
    succ = x;
    x = x->lChild;
  }
  return succ;
}

template <typename T> inline bool MyBinNode<T>::IsLeaf() const {
  return (!lChild && !rChild);
}

/// @brief 逆时针旋转节点 更新所有节点的树高 自动连接上层
/// @return 返回旋转完成后的根
template <typename T> BinNodePosi<T> MyBinNode<T>::Zag() {
  BinNodePosi<T> rc = rChild;
  rc->parent = this->parent;
  if (rc->parent) {
    if (this == rc->parent->lChild) {
      rc->parent->lChild = rc;
    } else {
      rc->parent->rChild = rc;
    }
  }
  rChild = rc->lChild;
  if (rChild) {
    rChild->parent = this;
  }
  this->parent = rc;
  rc->lChild = this;

  // 连接完成 更新高度
  // height = 1 + std::max(stature(lChild), stature(rChild));
  // rc->height = 1 + std::max(stature(rc->lChild), stature(rc->rChild));
  // for (BinNodePosi<T> x = rc->parent; x; x = parent) {
  //   int h = 1 + std::max(stature(x->lChild), stature(x->rChild));
  //   if (x->height == h) {
  //     break;
  //   }
  //   x->height = h;
  // }

  return rc;
}

/// @brief 顺时针 自动连接上层
/// @return 返回旋转完成后的根
template <typename T> BinNodePosi<T> MyBinNode<T>::Zig() {
  BinNodePosi<T> lc = lChild;
  lc->parent = parent;
  if (lc->parent) {
    if (lc->parent->lChild == this) {
      lc->parent->lChild = lc;
    } else {
      lc->parent->rChild = lc;
    }
  }
  lChild = lc->rChild;
  if (lChild) {
    lChild->parent = this;
  }
  this->parent = lc;
  lc->rChild = this;

  // 连接完成 更新高度
  // height = 1 + std::max(stature(lChild), stature(rChild));
  // lc->height = 1 + std::max(stature(lc->lChild), stature(lc->rChild));
  // for (BinNodePosi<T> x = lc->parent; x; x = parent) {
  //   int h = 1 + std::max(stature(x->lChild), stature(x->rChild));
  //   if (x->height == h) {
  //     break;
  //   }
  //   x->height = h;
  // }

  return lc;
}

#endif