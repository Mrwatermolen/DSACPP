#ifndef MYBST_HPP
#define MYBST_HPP
#include "MyBinTree.hpp"

// 返回引用最常见的是返回类的成员的引用，这些成员可能本身是某些private/protected字段的一部分，但在符合条件时允许外部直接修改，容器类当中很常见。

/// @brief 二叉搜索树
/// @tparam T
template <typename T>
class MyBST : public MyBinTree<T> {
 private:
 protected:
  BinNodePosi<T> _hot;  // 命中节点的父亲
  BinNodePosi<T> connect34(BinNodePosi<T>, BinNodePosi<T>, BinNodePosi<T>,
                           BinNodePosi<T>, BinNodePosi<T>, BinNodePosi<T>,
                           BinNodePosi<T>);
  BinNodePosi<T> rotateAt(BinNodePosi<T>);

  static BinNodePosi<T> &searchIn(BinNodePosi<T> &, const T &,
                                  BinNodePosi<T> &);
  static BinNodePosi<T> removeAt(BinNodePosi<T> &, BinNodePosi<T> &);

 public:
  BinNodePosi<T> &Search(const T &);
  virtual BinNodePosi<T> Insert(const T &);
  virtual bool Remove(const T &);

  BinNodePosi<T> Maximum(BinNodePosi<T>);
  BinNodePosi<T> Minimum(BinNodePosi<T>);
  MyBST(const T &r) : MyBinTree<T>(r) { _hot = nullptr; };
};

/// @brief zag以及zig操作都可以统一归入到3+4重构中去
/// @tparam T
/// @param a 中序遍历下最先被访问的节点
/// @param b 中序遍历下中间被访问的节点
/// @param c 中序遍历下最后被访问的节点
/// @param T0 重构完成后节点a的左子树
/// @param T1 重构完成后节点a的右子树
/// @param T2 重构完成后节点c的左子树
/// @param T3 重构完成后节点c的右子树
/// @return 重构完成后的根节点
template <typename T>
BinNodePosi<T> MyBST<T>::connect34(BinNodePosi<T> a, BinNodePosi<T> b,
                                   BinNodePosi<T> c, BinNodePosi<T> T0,
                                   BinNodePosi<T> T1, BinNodePosi<T> T2,
                                   BinNodePosi<T> T3) {
  a->lChild = T0;
  a->rChild = T1;
  if (T0) {
    T0->parent = a;
  }
  if (T1) {
    T1->parent = a;
  }
  this->updateHeight(a);
  c->lChild = T2;
  c->rChild = T3;
  if (T2) {
    T2->parent = c;
  }
  if (T3) {
    T3->parent = c;
  }
  this->updateHeight(c);
  b->lChild = a;
  b->rChild = c;
  if (a) {
    a->parent = b;
  }
  if (c) {
    c->parent = b;
  }
  this->updateHeight(b);
  return b;
}

/// @brief 统一接管旋转操作 子树的根会正确指向上层 但反向连接由上层完成
/// @tparam T
/// @param v 插入删除操作后首个被破坏平衡的节点
/// @return 旋转完成后的树的根
template <typename T>
BinNodePosi<T> MyBST<T>::rotateAt(BinNodePosi<T> v) {
  BinNodePosi<T> p = v->parent, g = p->parent;

  if ((p == g->rChild)) {
    // zag
    if (v == p->rChild) {
      // zag
      p->parent = g->parent;
      return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
    } else {
      // zig
      v->parent = g->parent;
      return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
    }
  } else {
    // zig
    if (v == p->lChild) {
      // zig
      p->parent = g->parent;
      return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
    } else {
      // zag
      v->parent = g->parent;
      return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
    }
  }
}

/// @brief 课程使用static修饰 为何
/// @param x 树根x
/// @param k 目标关键码
/// @param hot 记忆热点
/// @return 节点指针的引用  引用为了方便Insert操作
template <typename T>
BinNodePosi<T> &MyBST<T>::searchIn(BinNodePosi<T> &x, const T &k,
                                   BinNodePosi<T> &hot) {
  /* 错误的迭代
   while (x && x->data != k) {
    hot = x;
    if (x->data < k) {
      x = x->rChild;
    } else {
      x = x->lChild;
    }
  }
  return x; */
  if (!x || x->data == k) {
    return x;
  }
  hot = x;
  if (x->data < k) {
    return searchIn(x->rChild, k, hot);
  } else {
    return searchIn(x->lChild, k, hot);
  }
}

/// @brief 删除节点x 自动连接上下 TODO:release
/// @param x 删除的节点的引用
/// @param hot 记忆热点
/// @return 返回被删除节点的接替者
template <typename T>
BinNodePosi<T> MyBST<T>::removeAt(BinNodePosi<T> &x, BinNodePosi<T> &hot) {
  BinNodePosi<T> w = x;           // w总是被删除节点的备份
  BinNodePosi<T> succ = nullptr;  // 删除节点的接替者
  if (!x->lChild) {
    // 左孩子为空
    x = x->rChild;  // 在树中删去x 使用右孩子代替其位置
    succ = x;       // 记录接替者
  } else if (!x->rChild) {
    // 右子树为空
    x = x->lChild;
    succ = x;
  } else {
    // 左右子树非空
    // 不能直接使用引用删除
    // 找到x的直接后继 交换两者的数据后删去直接后继
    // 此时被删除的节点为x的直接后继
    w = w->Succ();
    swap(x->data, w->data);
    BinNodePosi<T> u = w->parent;
    if (u == x) {
      // x的直接后继为其的右子树的根
      // 说明直接后继的左子树为空
      u->rChild = w->rChild;
      succ = w->rChild;
    } else {
      // 一般情况
      // w为x的直接后继 说明w左子树为空
      // u的左子树要替换为w的右子树
      u->lChild = w->rChild;
      succ = w->rChild;
    }
  }
  hot = w->parent;
  if (succ) {
    succ->parent = hot;
  }
  // release(w);
  // release(w->data);
  return succ;
}

/// @brief 树的_hot总是命中命中节点的父亲
/// @param k 要查询的关键码
/// @return 成功返回指向关键码为e且真实存在的节点的引用 失败时
/// 指向最后一次试图转向的空节点
template <typename T>
BinNodePosi<T> &MyBST<T>::Search(const T &k) {
  return searchIn(this->_root, k, _hot = nullptr);
}

/// @brief 将关键字插入BST树中
/// @param e 插入的关键字
/// @return 插入的节点
template <typename T>
BinNodePosi<T> MyBST<T>::Insert(const T &e) {
  BinNodePosi<T> &x = Search(e);  // 获得插入位置的引用
  if (x) {
    // 防止雷同元素
    return x;
  }
  x = new MyBinNode<T>(e, _hot);
  this->_size++;
  this->updateHeightAbove(x);
  return x;
}

/// @brief 删除关键码为e的节点
/// @tparam T
/// @param e
/// @return 是否删除成功
template <typename T>
bool MyBST<T>::Remove(const T &e) {
  BinNodePosi<T> &x = Search(e);
  if (!x) {
    return false;
  }
  removeAt(x, _hot);
  this->_size--;
  this->updateHeightAbove(_hot);
  return true;
}

/// @brief
/// @param x 树x
/// @return 返回树x的最大关键字节点
template <typename T>
BinNodePosi<T> MyBST<T>::Maximum(BinNodePosi<T> x) {
  while (x->rChild) {
    x = x->rChild;
  }
  return x;
}

/// @brief
/// @param x 树x
/// @return 返回树x的最小关键字节点
template <typename T>
BinNodePosi<T> MyBST<T>::Minimum(BinNodePosi<T> x) {
  while (x->lChild) {
    x = x->lChild;
  }
  return x;
}

#endif