#ifndef MYHUFFMANTREE_H
#define MYHUFFMANTREE_H
#include "MyBinTree.hpp"
#include "MyPriorityQueue.hpp"
#include "MyVector.hpp"

#include <map>
#include <string>

struct Entry {
  char c;
  int f;
  std::string code;
  Entry(char c = 0, int f = 0, std::string s = "") : c(c), f(f), code(s) {}
  Entry(const Entry &e) { *this = e; }
  Entry &operator=(const Entry &e) {
    if (this == &e) {
      return *this;
    }
    c = e.c, f = e.f, code = e.code;
    return *this;
  }
  bool operator<(const Entry &e) const { return this->f < e.f; }
  bool operator<=(const Entry &e) const { return this->f <= e.f; }
  bool operator>(const Entry &e) const { return this->f > e.f; }
  bool operator>=(const Entry &e) const { return this->f >= e.f; }
  bool operator!=(const Entry &e) const { return this->f != e.f; }
  bool operator==(const Entry &e) const { return this->f != e.f; }
};

using VocabTable = MyVector<Entry>;
using HCodeBook = std::map<char, std::string>;

class MyHuffmanTree : public MyBinTree<Entry> {
protected:
  void buildHaffumanCode(BinNodePosi<Entry> x, std::string code);

public:
  MyHuffmanTree(const VocabTable &v);
  HCodeBook GetCodeBook() const;
};
#endif