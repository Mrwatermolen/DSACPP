#ifndef MYHUFFMANCODER_H
#define MYHUFFMANCODER_H
#include "MyHuffmanTree.h"
#include <fstream>

VocabTable textCharCounter(const string &file);

class MyHuffmanCoder {
protected:
  void decodeIn8Bit(unsigned char binC, BinNodePosi<Entry> &x,
                    const BinNodePosi<Entry> hRoot, ofstream &writeFile);

public:
  MyHuffmanCoder() {}
  bool Encode(const string &textFile, const string &saveFile,
              const MyHuffmanTree &huffTree);
  bool Decode(const string &binFile, const string &saveFile,
              const MyHuffmanTree &huffTree);
};

#endif