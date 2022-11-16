#include "MyHuffmanCoder.h"
#include "MyHuffmanTree.h"
#include "testMyHuffmanCoder.h"

void testMyHuffmanCoder() {
  std::cout << "Input Path" << std::endl;
  std::string rawPath, encodePath, decodePath;
  // std::getline(std::cin, rawPath;
  // std::getline(std::cin, encodePath;
  // std::getline(std::cin, decodePath);
  VocabTable m = textCharCounter(rawPath);
  MyHuffmanTree huffmanTree(m);
  MyHuffmanCoder coder;
  coder.Encode(rawPath, encodePath, huffmanTree);
  coder.Decode(encodePath, decodePath, huffmanTree);
}