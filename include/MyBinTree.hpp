#ifndef MYBINTREE_HPP
#define MYBINTREE_HPP
#include "MyBinNode.hpp"
#include "MyQueue.hpp"
#include "MyStack.hpp"

template <typename T>
class MyBinTree {
 private:
 protected:
  int _size;
  BinNodePosi(T) _root;
  virtual int updateHeight(BinNodePosi(T) x);
  void updateHeightAbove(BinNodePosi(T) x);

 public:
  MyBinTree(const T& e);
  ~MyBinTree();

  int Size();
  bool Empty();
  BinNodePosi(T) Root();
  BinNodePosi(T) InsertAsLC(const T& e, BinNodePosi(T) x);
  BinNodePosi(T) InsertAsRC(const T& e, BinNodePosi(T) x);

  template <typename VST>
  void PreOrderTraverse(BinNodePosi(T) x, VST& visit);
  template <typename VST>
  void InOrderTraverse(BinNodePosi(T) x, VST& visit);
  template <typename VST>
  void PostOrderTraverse(BinNodePosi(T) x, VST& visit);
  template <typename VST>
  void LevelOrderTraverse(BinNodePosi(T) x, VST& visit);
};

template <typename T>
MyBinTree<T>::MyBinTree(const T& e) : _size(1) {
  _root = new MyBinNode<T>(e);
}

template <typename T>
MyBinTree<T>::~MyBinTree() {
  auto clear = [](auto&& self, BinNodePosi(T) x) -> bool {
    if (x) {
      self(self, x->lChild);
      self(self, x->rChild);
      delete x;
      x = nullptr;
    }
    return true;
  };
  clear(clear, _root);
}

//
// 更新x的高度 返回更新后的高度
//
template <typename T>
int MyBinTree<T>::updateHeight(BinNodePosi(T) x) {
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

//
// 更新x以及其祖先的高度
//
template <typename T>
void MyBinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
  while (x) {
    int h = x->height;
    if (updateHeight(x) == h) {
      break;
    }
    x = x->parent;
  }
}

template <typename T>
int MyBinTree<T>::Size() {
  return _size;
}

template <typename T>
bool MyBinTree<T>::Empty() {
  return !_root;
}

template <typename T>
BinNodePosi(T) MyBinTree<T>::Root() {
  return _root;
}

//   BinNodePosi(T) InsertAsLC(BinNodePosi(T) x);
// BinNodePosi(T) InsertAsRC(BinNodePosi(T) x);

//
// 作为x的左孩子插入
//
template <typename T>
BinNodePosi(T) MyBinTree<T>::InsertAsLC(const T& e, BinNodePosi(T) x) {
  _size++;
  x->InsertAsLC(e);
  updateHeightAbove(x);
  return x->lChild;
}

//
// 作为x的右孩子插入
//
template <typename T>
BinNodePosi(T) MyBinTree<T>::InsertAsRC(const T& e, BinNodePosi(T) x) {
  _size++;
  x->InsertAsRC(e);
  updateHeightAbove(x);
  return x->rChild;
}

//
// 前序遍历
//
template <typename T>
template <typename VST>
void MyBinTree<T>::PreOrderTraverse(BinNodePosi(T) x, VST& visit) {
  MyStack<BinNodePosi(T)> S;
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

//
// 中序遍历
//
template <typename T>
template <typename VST>
void MyBinTree<T>::InOrderTraverse(BinNodePosi(T) x, VST& visit) {
  MyStack<BinNodePosi(T)> S;
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

//
// 后序遍历
//
template <typename T>
template <typename VST>
void MyBinTree<T>::PostOrderTraverse(BinNodePosi(T) x, VST& visit) {
  MyStack<BinNodePosi(T)> S;
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
      BinNodePosi(T) p = S.Pop();
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

//
// 层序遍历
//
template <typename T>
template <typename VST>
void MyBinTree<T>::LevelOrderTraverse(BinNodePosi(T) x, VST& visit) {
  if (!x) {
    return;
  }
  MyQueue<BinNodePosi(T)> Q;
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