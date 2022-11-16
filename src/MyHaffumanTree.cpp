#include "MyHuffmanTree.h"
#include <string>

MyHuffmanTree::MyHuffmanTree(const VocabTable &v) {
  if (v.Empty()) {
    return;
  }
  MyVector<MyBinNode<Entry>> vt;
  for (int i = 0; i < v.Size(); i++) {
    vt.Insert(vt.Size(), MyBinNode<Entry>(v[i]));
  }
  MyPQCompleteHeap<MyBinNode<Entry>, MinHeap<MyBinNode<Entry>>> *pQ =
      new MyPQCompleteHeap<MyBinNode<Entry>, MinHeap<MyBinNode<Entry>>>(vt);
  while (!pQ->Empty()) {
    BinNodePosi<Entry> x = new MyBinNode<Entry>(pQ->DelMax());
    if (pQ->Empty()) {
      if (!this->_root) {
        this->_root = x;
        break;
      }
      delete x;
      break;
    }

    BinNodePosi<Entry> y = new MyBinNode<Entry>(pQ->DelMax());
    BinNodePosi<Entry> newRoot =
        new MyBinNode<Entry>(Entry(0, x->data.f + y->data.f), nullptr, x, y);
    x->parent = newRoot;
    y->parent = newRoot;
    this->_root = newRoot;
    if (pQ->Empty()) {
      break;
    }
    pQ->Insert(*(this->_root));
  }
  delete pQ;
  buildHaffumanCode(this->_root, std::string(""));
}

void MyHuffmanTree::buildHaffumanCode(BinNodePosi<Entry> x, std::string code) {
  if (!x) {
    return;
  }

  if (x->IsLeaf()) {
    x->data.code = code;
    return;
  }
  if (x->lChild) {
    buildHaffumanCode(x->lChild, code + std::string("0"));
  }
  if (x->rChild) {
    buildHaffumanCode(x->rChild, code + std::string("1"));
  }
}

HCodeBook MyHuffmanTree::GetCodeBook() const {
  HCodeBook book;

  auto preOrder = [&book](auto seft, BinNodePosi<Entry> x) -> bool {
    if (!x) {
      return true;
    }
    if (x->IsLeaf()) {
      book[x->data.c] = x->data.code;
      return true;
    }
    seft(seft, x->lChild);
    seft(seft, x->rChild);
    return true;
  };

  preOrder(preOrder, this->_root);
  return book;
}