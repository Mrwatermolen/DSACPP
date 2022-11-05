#ifndef MYBITMAP_H
#define MYBITMAP_H
#include <iostream>
#define MyBitmapDebug true

class MyBitmap {
 private:
  int N;
  // M中的一个char可以存储8种不同的状态
  // 对一个key, 将其高位直至低位的第四位映射到M元素的索引
  char* M;

 public:
  MyBitmap(int n = 8);
  ~MyBitmap();

  MyBitmap& operator=(const MyBitmap& b);

  void Set(int k);
  void Claer(int k);
  bool Test(int k);
};
#endif