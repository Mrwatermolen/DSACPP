#ifndef MYREDBLACK_HPP
#define MYREDBLACK_HPP
#include "MyBST.hpp"
#include "MyBinNode.hpp"
#include <algorithm>

template <typename T> class MyRedBlack : public MyBST<T> {
protected:
  int updateHeight(BinNodePosi<T>);
  void doubleRed(BinNodePosi<T>);
  void doubleBlack(BinNodePosi<T>, BinNodePosi<T>);
  void blackBlack1(BinNodePosi<T>, BinNodePosi<T>);

public:
  BinNodePosi<T> Insert(const T &);
  bool Remove(const T &);
};

///@return 返回以x为根的子树的黑高度
template <typename T> int MyRedBlack<T>::updateHeight(BinNodePosi<T> x) {
  x->height = std::max(0, std::max(stature(x->lChild), stature(x->rChild)));
  if (isBlack(x)) {
    ++x->height;
  }
  return x->height;
}

///@brief 红黑树删除情况的BB-1 进行旋转操作
///@param v 满足BB1中的红孩子
///@param p 旋转影响的祖先 用于旋转后与p的父亲的连接
template <typename T>
void MyRedBlack<T>::blackBlack1(BinNodePosi<T> v, BinNodePosi<T> p) {
  BinNColor c = p->color; // 备份颜色
  v = this->rotateAt(v);
  v->color = c;
  v->lChild->color = BLACK;
  v->rChild->color = BLACK;
  this->updateHeight(v->lChild);
  this->updateHeight(v->rChild);
  this->updateHeight(v);
  // 连接
  if (v->parent) {
    if (v->parent->lChild == p) {
      v->parent->lChild = v;
    } else {
      v->parent->rChild = v;
    }
  } else {
    this->_root = v;
  }
}

///@brief 判断是否出现红黑树插入后出现的双红缺陷以及解决
///@param v 新插入的节点
template <typename T> void MyRedBlack<T>::doubleRed(BinNodePosi<T> v) {
  while (v != this->_root) {
    if (isBlack(v)) {
      return;
    }
    BinNodePosi<T> p = v->parent;
    if (isBlack(p)) {
      return;
    }
    // 出现双红缺陷 p节点必定存在黑父亲
    BinNodePosi<T> g = p->parent,
                   u = (g->lChild != p) ? (g->lChild) : (g->rChild);
    if (isBlack(u)) {
      // 旋转操作即可 把中间节点与节点g交换即可 看作为一颗4阶B树
      v = this->rotateAt(v);
      v->color = RED;
      v->lChild->color = BLACK;
      v->rChild->color = BLACK;
      // 完成旋转后的连接
      if (v->parent) {
        if (v->parent->lChild == g) {
          v->parent->lChild = v;
        } else {
          v->parent->rChild = v;
        }
      } else {
        v->color = BLACK;
        this->_root = v;
      }
    } else {
      // 重新染色
      g->color = RED;
      p->color = BLACK;
      u->color = BLACK;
      v = g;
    }
    updateHeight(v->lChild);
    updateHeight(v->rChild);
    updateHeight(v);
  }
  v->color = BLACK; // 到达树根 树根不能为红 分裂
  updateHeight(v);
  return;
}

///@brief 解决红黑树的删除后出现的双黑缺陷
///@param p 红黑树中第一个出现不平衡的节点 v的父亲
///@param v 被删除节点的接替者
template <typename T>
void MyRedBlack<T>::doubleBlack(BinNodePosi<T> p, BinNodePosi<T> v) {
  while (p) {
    BinNodePosi<T> s =
        (p->lChild != v) ? (p->lChild) : (p->rChild); // v的兄弟节点
    if (isBlack(s)) {
      // BB-1
      // 兄弟s为黑且s拥有一个红孩子
      // 相当于 B树的下溢 但可向兄弟节点借一个节点
      if (!isBlack(s->lChild)) {
        blackBlack1(s->lChild, p);
        return;
      } else if (!isBlack(s->rChild)) {
        blackBlack1(s->rChild, p);
        return;
      }

      // BB-2R
      // 兄弟s为黑且s孩子节点全为黑
      // 父节点p为红
      // 相当于B树中 s p x(被删除 视作空)合并为一个节点
      if (!isBlack(p)) {
        s->color = RED;
        p->color = BLACK;
        updateHeight(s);
        updateHeight(p);
        return;
      }

      // BB-2B
      // 兄弟s为黑且s孩子节点全为黑
      // 父节点为黑
      // 相当于B树中 取出p合并后 父节点又下溢
      // 这就相当于又出现一次双黑缺陷
      if (isBlack(p)) {
        s->color = RED;
        p->color = BLACK;
        updateHeight(s);
        updateHeight(p);
        if (p->parent) {
          updateHeight(p->parent);
        }

        v = p; // 受影响的节点为p
        p = p->parent;
        continue;
      }
    }
    // BB-3
    // 兄弟s为红
    if (!isBlack(s)) {
      // 其父节点p肯定为黑
      if (p->lChild == s) {
        p->Zig();
      } else {
        p->Zag();
      }
      p->color = RED;
      s->color = BLACK;
      if (p == this->_root) {
        this->_root = s;
      }
      updateHeight(p);
      updateHeight(s);
      continue;
      // 有可能出现双黑缺陷
      // 此时对于v而言拥有了一个黑兄弟s'
    }
  }
}

template <typename T> BinNodePosi<T> MyRedBlack<T>::Insert(const T &e) {
  BinNodePosi<T> &x = this->Search(e);
  if (x) {
    return x;
  }
  x = new MyBinNode<T>(e, this->_hot, nullptr, nullptr, 0, RED);
  doubleRed(x);
  this->_size++;
  return x;
}

template <typename T> bool MyRedBlack<T>::Remove(const T &e) {
  BinNodePosi<T> &x = this->Search(e);
  if (!x) {
    return false;
  }
  BinNodePosi<T> r = this->removeAt(x, this->_hot);
  --this->_size;
  if (!this->_hot) {
    // 来到了根节点
    // 对应的情况为要删除的值为根节点的值 且根节点只有小于等于1个孩子
    if (!this->_root) {
      // 根节点也被删除
      return true;
    }
    this->_root->color = BLACK; // 剩下的肯定为红节点 根节点要为黑
    updateHeight(this->_root);
    return true;
  }
  // 接替者r为红色 说明实际被删除节点为黑 且删除后对B树而言不产生下溢
  if (!isBlack(r)) {
    r->color = BLACK;
    updateHeight(r);
    return true;
  }
  // 检查_hot左右两边的黑高度
  // 黑高度不变说明被删除的节点颜色为红色
  BinNodePosi<T> s =
      this->_hot->lChild != r ? this->_hot->lChild : this->_hot->rChild;
  int rH = (r) ? (r->height) : (0);
  int sH = (s) ? (s->height) : (0);
  if (sH == rH) {
    return true;
  }
  // 双黑缺陷 接替者r以及被删除的节点都为黑色
  doubleBlack(this->_hot, r);
  return true;
}

#endif