#include "MyBitmap.h"

MyBitmap::MyBitmap(int n) : N((n + 7) / 8) {
  M = new char[N];
  for (int i = 0; i < N; i++) {
    M[i] = 0x00;
  }
}

MyBitmap::~MyBitmap() {
  if (M != nullptr) {
    delete[] M;
    M = nullptr;
  }
}

MyBitmap& MyBitmap::operator=(const MyBitmap& b) {
  if (this == &b) {
    return *this;
  }
  if (M) {
    delete[] M;
    M = nullptr;
  }
  N = b.N;
  M = new char[N];
  for (int i = 0; i < N; i++) {
    M[i] = b.M[i];
  }
  return *this;
}

void MyBitmap::Set(int k) {
  auto index = k >> 3;
  auto offset = k & 0x07;
  M[index] |= (0x80 >> offset);
}

void MyBitmap::Claer(int k) {
  auto index = k >> 3;
  auto offset = k & 0x07;
  M[index] &= ~(0x80 >> offset);
}

bool MyBitmap::Test(int k) {
  auto index = k >> 3;
  auto offset = k & 0x07;
  auto flag = 0x80 >> offset;
  return (M[index] & flag);
}