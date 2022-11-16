#include "MyHuffmanCoder.h"

VocabTable textCharCounter(const string &file) {
  VocabTable v;
  ifstream readFile(file);
  if (!readFile.is_open()) {
    return v;
  }
  map<char, int> f;
  readFile >> noskipws;
  while (true) {
    char c;
    readFile.get(c);
    if (readFile.eof()) {
      break;
    }
    if (!f.count(c)) {
      f[c] = 1;
      continue;
    }
    f[c]++;
  }
  for (auto &&i : f) {
    v.Insert(v.Size(), Entry(i.first, i.second));
  }
  return v;
}

bool MyHuffmanCoder::Encode(const string &textFile, const string &saveFile,
                            const MyHuffmanTree &huffTree) {
  if (huffTree.Empty()) {
    return false;
  }
  ifstream readFile(textFile);
  ofstream writeFile(saveFile, ios::binary | ios::trunc);
  if (!readFile.is_open() || !writeFile.is_open()) {
    return false;
  }

  HCodeBook codeBook = huffTree.GetCodeBook();
  int count = 0, times = 0;
  unsigned char binC = 0x00;
  char c;
  while ((c = readFile.get()) != EOF) {
    // std::cout << c;
    string code = codeBook[c];
    BinNodePosi<Entry> x = huffTree.Root();
    for (int i = 0; i < code.length(); i++) {
      switch (code[i]) {
      case '0': {
        x = x->lChild;
        binC = ((binC & 0x7f));
        break;
      }
      case '1': {
        x = x->rChild;
        binC = ((binC | 0x80));
        break;
      }
      }
      if (++count == 8) {
        times++;
        writeFile << binC;
        count = 0;
        binC = 0x00;
      }
      binC >>= 1;
    }
  }
  if (count != 0 && count < 7) {
    binC >>= (7 - count);
    writeFile << binC;
  }
  readFile.close();
  writeFile.close();
  return true;
}

void MyHuffmanCoder::decodeIn8Bit(unsigned char binC, BinNodePosi<Entry> &x,
                                  const BinNodePosi<Entry> hRoot,
                                  ofstream &writeFile) {
  for (int count = 0; count < 8;) {
    while (x && !x->IsLeaf() && count < 8) {
      if (binC & 0x01) {
        x = x->rChild;
      } else {
        x = x->lChild;
      }
      binC >>= 1;
      count++;
    }

    if (x && x->IsLeaf()) {
      // 一个字节内完成对一个字符的解码
      writeFile << x->data.c; // 写入
      x = hRoot;
    }
  }
}

bool MyHuffmanCoder::Decode(const string &binFile, const string &saveFile,
                            const MyHuffmanTree &huffTree) {
  if (huffTree.Empty()) {
    return false;
  }
  ifstream readFile(binFile, ios::binary);
  ofstream writeFile(saveFile, ios::trunc);
  if (!readFile.is_open() || !writeFile.is_open()) {
    return false;
  }

  unsigned char binC;
  BinNodePosi<Entry> x = huffTree.Root();
  binC = readFile.get();
  while ((!readFile.eof()) && x) {
    // 在一个字节内解码
    decodeIn8Bit(binC, x, huffTree.Root(), writeFile);
    // 读取一个字节 8bit
    binC = readFile.get(); // get到了0xff
  }
  readFile.close();
  writeFile.close();
  return true;
}