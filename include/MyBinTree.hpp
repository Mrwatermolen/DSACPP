#ifndef MYBINTREE_HPP
#define MYBINTREE_HPP
#include "MyBinNode.hpp"
#include "MyQueue.hpp"
#include "MyStack.hpp"

template <typename T> class MyBinTree {
private:
protected:
  int _size;            // 树的规模
  BinNodePosi<T> _root; // 树根
  virtual int updateHeight(BinNodePosi<T>);
  void updateHeightAbove(BinNodePosi<T>);

public:
  MyBinTree() : _size(0), _root(nullptr){};
  MyBinTree(const T &e);
  ~MyBinTree();

  int Size();
  bool Empty();
  BinNodePosi<T> Root();
  BinNodePosi<T> InsertAsLC(const T &, BinNodePosi<T>);
  BinNodePosi<T> InsertAsRC(const T &, BinNodePosi<T>);

  template <typename VST> void PreOrderTraverse(BinNodePosi<T>, VST &);
  template <typename VST> void InOrderTraverse(BinNodePosi<T>, VST &);
  template <typename VST> void PostOrderTraverse(BinNodePosi<T>, VST &);
  template <typename VST> void LevelOrderTraverse(BinNodePosi<T>, VST &);
};

/// @brief 以e的值为根
/// @tparam T
/// @param e
template <typename T> MyBinTree<T>::MyBinTree(const T &e) : _size(1) {
  _root = new MyBinNode<T>(e);
}

template <typename T> MyBinTree<T>::~MyBinTree() {
  auto clear = [](auto &&self, BinNodePosi<T> x) -> bool {
    if (x) {
      self(self, x->lChild);
      self(self, x->rChild);
      delete x;
      x = nullptr;
    }
    return true;
  };
  clear(clear, _root);
  _size = 0;
}

/// @brief 更新树x的高度
/// @tparam T
/// @param x 树根
/// @return 返回更新后树x的高度
template <typename T> int MyBinTree<T>::updateHeight(BinNodePosi<T> x) {
  x->height = 1 + std::max(stature(x->lChild), stature(x->rChild));
  return x->height;
}

/// @brief 更新树x以及其祖先的高度
/// @tparam T
/// @param x 树的根x
template <typename T> void MyBinTree<T>::updateHeightAbove(BinNodePosi<T> x) {
  while (x) {
    int h = x->height;
    if (updateHeight(x) == h) {
      break;
    }
    x = x->parent;
  }
}

/// @brief
/// @tparam T
/// @return 树的规模
template <typename T> int MyBinTree<T>::Size() { return _size; }

/// @brief 树是否为空
/// @tparam T
/// @return
template <typename T> bool MyBinTree<T>::Empty() { return !_root; }

/// @brief
/// @tparam T
/// @return 返回树的根
template <typename T> BinNodePosi<T> MyBinTree<T>::Root() { return _root; }

/// @brief 作为x的左孩子插入 要求x的左孩子为空 不做检查
/// @tparam T
/// @param e
/// @param x 树的节点x
/// @return x的左孩子
template <typename T>
BinNodePosi<T> MyBinTree<T>::InsertAsLC(const T &e, BinNodePosi<T> x) {
  _size++;
  x->InsertAsLC(e);
  updateHeightAbove(x);
  return x->lChild;
}

/// @brief 作为x的右孩子插入 要求x的右孩子为空 不做检查
/// @tparam T
/// @param e
/// @param x 树的节点x
/// @return x的右孩子
template <typename T>
BinNodePosi<T> MyBinTree<T>::InsertAsRC(const T &e, BinNodePosi<T> x) {
  _size++;
  x->InsertAsRC(e);
  updateHeightAbove(x);
  return x->rChild;
}

/// @brief 以节点x为树根的前序遍历
/// @tparam T
/// @param x 树根x
/// @param visit 遍历器
template <typename T>
template <typename VST>
void MyBinTree<T>::PreOrderTraverse(BinNodePosi<T> x, VST &visit) {
  MyStack<BinNodePosi<T>> S;
  while (true) {
    while (x) {
      visit(x->data);
      if (x->rChild) {
        S.Push(x->rChild);
      }
      x = x->lChild;
    }

    if (S.Empty()) {
      break;
    }
    x = S.Pop();
  }
}

/// @brief 以节点x为树根的中序遍历
/// @tparam T
/// @param x 树根x
/// @param visit 遍历器
template <typename T>
template <typename VST>
void MyBinTree<T>::InOrderTraverse(BinNodePosi<T> x, VST &visit) {
  MyStack<BinNodePosi<T>> S;
  while (true) {
    while (x) {
      S.Push(x);
      x = x->lChild;
    }
    if (S.Empty()) {
      break;
    }
    x = S.Pop();
    visit(x->data);
    x = x->rChild;
  }
}

/// @brief 以节点x为树根的后序遍历
/// @tparam T
/// @param x 树根x
/// @param visit 遍历器
template <typename T>
template <typename VST>
void MyBinTree<T>::PostOrderTraverse(BinNodePosi<T> x, VST &visit) {
  MyStack<BinNodePosi<T>> S;
  bool isLeft = false;
  while (true) {
    while (x) {
      S.Push(x);
      x = x->lChild;
      isLeft = true;
    }

    if (S.Empty()) {
      break;
    }
    if (isLeft) {
      x = S.Top()->rChild;
      isLeft = false;
      continue;
    }

    while (!S.Empty() && x == S.Top()->rChild) {
      BinNodePosi<T> p = S.Pop();
      visit(p->data);
      x = p;
    }
    if (S.Empty()) {
      x = nullptr;
    } else {
      x = S.Top()->rChild;
    }
    isLeft = true;
  }
}

/// @brief 以节点x为树根的层序遍历
/// @tparam T
/// @param x 树根x
/// @param visit 遍历器
template <typename T>
template <typename VST>
void MyBinTree<T>::LevelOrderTraverse(BinNodePosi<T> x, VST &visit) {
  if (!x) {
    return;
  }
  MyQueue<BinNodePosi<T>> Q;
  Q.Enqueue(x);
  while (!Q.Empty()) {
    x = Q.Dequeue();
    visit(x->data);
    if (x->lChild) {
      Q.Enqueue(x->lChild);
    }
    if (x->rChild) {
      Q.Enqueue(x->rChild);
    }
  }
}

#endif