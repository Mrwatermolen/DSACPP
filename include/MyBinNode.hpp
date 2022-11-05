#ifndef MYBINNODE_HPP
#define MYBINNODE_HPP
#define BinNodePosi(T) MyBinNode<T>*

template <typename T>
struct MyBinNode {
  T data;
  BinNodePosi(T) lChild;
  BinNodePosi(T) rChild;
  BinNodePosi(T) parent;  // 孩子 父亲
  int height;             // 高度

  int Size();
  BinNodePosi(T) InsertAsLC(const T& e);
  BinNodePosi(T) InsertAsRC(const T& e);
  BinNodePosi(T) Succ();  // 中序遍历下的节点的后继

  MyBinNode(const T& e, BinNodePosi(T) p = nullptr)
      : data(e), parent(p), lChild(nullptr), rChild(nullptr){};
};

//
// 子树的规模
//
template <typename T>
int MyBinNode<T>::Size() {
  int s = 1;
  if (lChild) {
    s += lChild->Size();
  }
  if (rChild) {
    s += rChild->Size();
  }
  return s;
}

//
// 作为左孩子插入
//
template <typename T>
BinNodePosi(T) MyBinNode<T>::InsertAsLC(const T& e) {
  return (lChild = new MyBinNode(e, this));
}

//
// 作为右孩子插入
//
template <typename T>
BinNodePosi(T) MyBinNode<T>::InsertAsRC(const T& e) {
  return (rChild = new MyBinNode(e, this));
}

#endif