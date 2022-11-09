#ifndef MYBTREE_HPP
#include "MyBTreeNode.hpp"
#include "MyQueue.hpp"
#include "MyVector.hpp"

template <typename T> class MyBTree {
private:
  int _size;          // 规模
  int _order;         // 阶数
  BNodePosi<T> _root; // 树根
  BNodePosi<T> _hot;  // 热点

protected:
  void solveOverflow(BNodePosi<T>);
  void solveUnderflow(BNodePosi<T>);

public:
  MyBTree(int order, T r) : _size(1), _order(order), _hot(nullptr) {
    _root = new MyBTreeNode<T>;
    _root->key.Insert(0, r);
    _root->child.Insert(0, nullptr);
    _root->child.Insert(0, nullptr);
  }

  BNodePosi<T> Root() { return _root; }
  BNodePosi<T> Search(const T &);
  bool Insert(const T &);
  bool Remove(const T &);

  template <typename VST> void LevelOrderTraverse(BNodePosi<T>, VST &);
  template <typename VST> void InOrderTraverse(BNodePosi<T>, VST &);
};

/// @brief 检查是否发生下溢并处理
/// @param x 可能下 溢的节点
template <typename T> void MyBTree<T>::solveOverflow(BNodePosi<T> x) {
  while (x && (_order < x->child.Size())) {
    int mid = x->key.Size() / 2;
    T midKey = x->key[mid]; // 要于父亲节点合并的值

    BNodePosi<T> rc = new MyBTreeNode<T>; // 分裂出的右孩子
    MyVector<T> rcKey(x->key, mid + 1, x->key.Size());
    MyVector<BNodePosi<T>> rcChild(x->child, mid + 1, x->child.Size());
    for (int i = 0; i < rcChild.Size() && rcChild[i]; i++) {
      rcChild[i]->parent = rc;
    }
    rc->key = rcKey, rc->child = rcChild;

    x->key.Remove(mid, x->key.Size());
    x->child.Remove(mid + 1, x->child.Size());

    if (x->parent) {
      x = x->parent;
    } else {
      BNodePosi<T> p = new MyBTreeNode<T>;
      x->parent = p;
      p->child.Insert(0, x);
      x = p;
      this->_root = x;
    }
    Rank pos = x->key.Search(midKey, 0, x->key.Size()) + 1;
    x->key.Insert(pos, midKey);
    // 将右孩子加入
    rc->parent = x;
    x->child.Insert(pos + 1, rc);
  }
}

template <typename T> void MyBTree<T>::solveUnderflow(BNodePosi<T> x) {
  while (x != _root && x && x->child.Size() < ((_order + 1) / 2)) {
    BNodePosi<T> p = x->parent, lc = nullptr, rc = nullptr;
    // Rank r = p->child.Search(x, 0, p->child.Size());
    Rank r = p->child.Find(x, 0, p->child.Size());
    // Rank r = p->
    // 向兄弟节点借一个元素
    if (0 < r) {
      // 尝试向左兄弟借
      lc = p->child[r - 1];
      if (((_order + 1) / 2) < lc->child.Size()) {
        // 可借
        x->key.Insert(0, p->key[r - 1]);
        p->key[r - 1] = lc->key[lc->key.Size() - 1];
        lc->key.RemoveAt(lc->key.Size() - 1);

        x->child.Insert(0, lc->child[lc->child.Size() - 1]);
        lc->child.RemoveAt(lc->child.Size() - 1);
        return; // 完成
      }
    }
    if (r < p->child.Size() - 1) {
      // 尝试向右兄弟借
      rc = p->child[r + 1];
      if (((_order + 1) / 2) < rc->child.Size()) {
        // 可借
        x->key.Insert(x->key.Size(), p->key[r + 1]);
        p->key[r + 1] = rc->key[0];
        rc->key.RemoveAt(0);

        x->child.Insert(x->child.Size(), rc->child[0]);
        rc->child.RemoveAt(0);
        return;
      }
    }
    // 左右兄弟都处于临界状态 尝试合并
    if (0 < r) {
      // 尝试和左兄弟以及父亲合并
      lc = p->child[r - 1];
    } else {
      // 尝试和右兄弟以及父亲合并
      lc = x;
      x = p->child[r + 1];
      r = r + 1;
    }
    lc->key.Insert(lc->key.Size(), p->key[r - 1]);
    for (int i = 0; i < x->key.Size(); i++) {
      lc->key.Insert(lc->key.Size(), x->key[i]);
    }
    for (int i = 0; i < x->child.Size(); i++) {
      if (x->child[i]) {
        x->child[i]->parent = lc;
      }
      lc->child.Insert(lc->child.Size(), x->child[i]);
    }
    // 对父亲进行处理
    p->key.RemoveAt(r - 1);
    p->child.RemoveAt(r);
    delete x;
    x = p;
  }
  // 根节点的处理
  if (x == _root && x->key.Size() < 1) {
    x->child[0]->parent = nullptr;
    _root = x->child[0];
    delete x;
  }
}

/// @brief 从树根开始搜索包含e的节点
/// @param e
/// @return 包含e的节点 若树中不存在e 则返回空
template <typename T> BNodePosi<T> MyBTree<T>::Search(const T &e) {
  BNodePosi<T> x = _root;
  _hot = nullptr;
  while (x) {
    Rank r = x->key.Search(e, 0, x->key.Size());
    if (0 <= r && x->key[r] == e) {
      return x;
    }
    _hot = x;
    x = x->child[r + 1];
  }
  return nullptr;
}

/// @brief 向B树中插入元素e
/// @param e
/// @return 是否插入成功 成功返回true 如果树中存在元素e则插入失败
template <typename T> bool MyBTree<T>::Insert(const T &e) {
  BNodePosi<T> x = Search(e);
  if (x) {
    return false;
  }
  Rank r = _hot->key.Search(e, 0, _hot->key.Size());
  _hot->key.Insert(r + 1, e);
  _hot->child.Insert(r + 2, nullptr);

  solveOverflow(_hot);
  _size++;
  return true;
}

/// @brief 向B树中删除元素e
/// @param e
/// @return 是否删除成功 成功返回true 如果树中不存在元素e则删除失败
template <typename T> bool MyBTree<T>::Remove(const T &e) {
  BNodePosi<T> x = Search(e);
  if (!x) {
    return false;
  }
  // 判断是否为叶子节点
  Rank r = x->key.Search(e, 0, x->key.Size());
  if (!x->IsLeaf()) {
    // 寻找直接后继
    BNodePosi<T> succ = x->child[r + 1];
    while (!succ->IsLeaf()) {
      succ = succ->child[0];
    }
    x->key[r] = succ->key[0];
    x = succ;
    r = 0;
  }
  x->key.RemoveAt(r);
  x->child.RemoveAt(r);
  _size--;
  solveUnderflow(x);
  return true;
}

/// @brief B树的层序遍历
/// @tparam VST 遍历器
/// @param x 要遍历的子树的根
template <typename T>
template <typename VST>
void MyBTree<T>::LevelOrderTraverse(BNodePosi<T> x, VST &visit) {
  if (!x) {
    return;
  }
  MyQueue<BNodePosi<T>> Q;
  Q.Enqueue(x);
  while (!Q.Empty()) {
    x = Q.Dequeue();
    x->key.Traverse(visit);
    for (int i = 0; i < x->child.Size() && x->child[i]; i++) {
      Q.Enqueue(x->child[i]);
    }
    cout << endl;
  }
}

/// @brief B树的中序遍历
/// @tparam VST 遍历器
/// @param x 要遍历的子树的根
template <typename T>
template <typename VST>
void MyBTree<T>::InOrderTraverse(BNodePosi<T> x, VST &visit) {
  if (!x) {
    return;
  }
  for (int i = 0; i < x->child.Size() - 1; i++) {
    InOrderTraverse(x->child[i], visit);
    visit(x->key[i]);
    InOrderTraverse(x->child[i + 1], visit);
  }
}

#endif