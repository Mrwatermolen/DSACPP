#ifndef MYBTREENODE_HPP
#include "MyBinNode.hpp"
#include "MyVector.hpp"

template <typename T> struct MyBTreeNode;
template <typename T> using BNodePosi = MyBTreeNode<T> *;

template <typename T> struct MyBTreeNode {
  MyVector<T> key;
  MyVector<BNodePosi<T>> child;
  BNodePosi<T> parent;

  MyBTreeNode(const MyVector<T> &keys, MyVector<BNodePosi<T>> &childs,
              BinNodePosi<T> p = nullptr)
      : key(keys), child(childs), parent(p) {}
  MyBTreeNode() : key(), child(), parent(nullptr) {}

  bool IsLeaf() { return !child[0]; }
};

#endif