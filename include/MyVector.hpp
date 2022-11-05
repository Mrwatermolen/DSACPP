#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP
#include <iostream>
#include <random>
#define Rank int
#define DEFAULT_CAPACITY 3
#define MyVectorDebug true

using namespace std;

template <typename T>
class MyVector {
 private:
  Rank _capacity;
  Rank _size;
  T* _elem;

 protected:
  void copyFrom(const T* A, Rank lo, Rank hi);
  void expand();
  void shrink();
  Rank binSearchA(const T& e, Rank lo, Rank hi);
  Rank binSearchB(const T& e, Rank lo, Rank hi);
  Rank binSearchC(const T& e, Rank lo, Rank hi);
  Rank fibSearch(const T& e, Rank lo, Rank hi);
  void bubbleSort(Rank lo, Rank hi);
  void selectSort(Rank lo, Rank hi);
  void mergeSort(Rank lo, Rank hi);
  void heapSort(Rank lo, Rank hi);
  void quickSort(Rank lo, Rank hi);

 public:
  explicit MyVector(int c = DEFAULT_CAPACITY);
  MyVector(int size, const T& e);
  MyVector(const T* A, Rank lo, Rank hi);
  ~MyVector();

  T& operator[](Rank r);
  MyVector<T>& operator=(const MyVector<T>& v);

  Rank Size();
  bool Empty();
  Rank Insert(Rank r, const T& e);
  int Remove(Rank lo, Rank hi);
  T RemoveAt(Rank r);
  Rank Find(const T& e, Rank lo, Rank hi);

  template <typename VST>
  void Traverse(VST& visit);
  Rank Search(const T& e, Rank lo, Rank hi);
  void Sort(Rank lo, Rank hi);
};

template <typename T>
MyVector<T>::MyVector(int c) : _capacity(c), _size(0), _elem(new T[c]) {}

template <typename T>
MyVector<T>::MyVector(const T* A, Rank lo, Rank hi) {
  copyFrom(A, lo, hi);
}

template <typename T>
MyVector<T>::MyVector(int size, const T& e)
    : _capacity(2 * size), _size(size), _elem(new T[2 * size]) {
  for (Rank i = 0; i < _size; i++) {
    _elem[i] = e;
  }
}

template <typename T>
MyVector<T>::~MyVector() {
  if (_elem) {
    delete[] _elem;
    _elem = nullptr;
  }
}

template <typename T>
void MyVector<T>::copyFrom(const T* A, Rank lo, Rank hi) {
  _capacity = 2 * (hi - lo);
  _elem = new T[_capacity];
  _size = 0;
  while (lo < hi) {
    _elem[_size] = A[lo];
    _size++, lo++;
  }
}

//
// 扩容操作 加倍扩容
//
template <typename T>
void MyVector<T>::expand() {
  if (_size < _capacity) {
    return;
  }
  _capacity = max(_capacity, DEFAULT_CAPACITY) * 2;
  T* oldElem = _elem;
  _elem = new T[_capacity];
  for (int i = 0; i < _size; i++) {
    _elem[i] = oldElem[i];
  }
  delete[] oldElem;
}

//
// 缩容操作
//
template <typename T>
void MyVector<T>::shrink() {}

//
// 二分查找 A版本
//
template <typename T>
Rank MyVector<T>::binSearchA(const T& e, Rank lo, Rank hi) {
  // 向左的代价更小 1次比较
  while (lo < hi) {
    Rank mi = ((hi - lo) >> 1) + lo;
    if (e < _elem[mi]) {
      hi = mi;
    } else if (_elem[mi] < e) {
      lo = mi + 1;
    } else {
      return mi;
    }
  }
  return -1;
}

//
// 二分查找 B版本
// ASL更加稳定
//
template <typename T>
Rank MyVector<T>::binSearchB(const T& e, Rank lo, Rank hi) {
  while (1 < (hi - lo)) {
    Rank mi = ((hi - lo) >> 1) + lo;
    if (e < _elem[mi]) {
      hi = mi;
    } else {
      lo = mi;
    }
  }
  if (e == _elem[lo]) {
    return lo;
  }
  return -1;
}

//
// 二分查找 C版本
// 满足Search的语义
//
template <typename T>
Rank MyVector<T>::binSearchC(const T& e, Rank lo, Rank hi) {
  while (lo < hi) {
    Rank mi = ((hi - lo) >> 1) + lo;
    if (e < _elem[mi]) {
      hi = mi;
    } else {
      lo = mi + 1;
    }
  }
  return lo - 1;
}

//
// 起泡排序
//
template <typename T>
void MyVector<T>::bubbleSort(Rank lo, Rank hi) {
  if (MyVectorDebug) {
    cout << "bubbleSort" << endl;
  }
  auto bubble = [this](int lo, int hi) {
    Rank last = lo;
    while (++lo < hi) {
      if (this->_elem[lo - 1] < this->_elem[lo]) {
        continue;
      }
      swap(this->_elem[lo - 1], this->_elem[lo]);
      last = lo;
    }
    return last;
  };
  while ((lo < (hi = bubble(lo, hi))))
    ;
}

//
// 选择排序
//
template <typename T>
void MyVector<T>::selectSort(Rank lo, Rank hi) {
  if (MyVectorDebug) {
    cout << "selectSort" << endl;
  }
  while (lo < hi) {
    auto minR = lo;
    for (Rank i = lo + 1; i < hi; i++) {
      if (_elem[minR] <= _elem[i]) {
        continue;
      }
      minR = i;
    }
    swap(_elem[lo++], _elem[minR]);
  }
}

//
// 归并排序
//
template <typename T>
void MyVector<T>::mergeSort(Rank lo, Rank hi) {
  if (hi - lo < 2) {
    return;
  }

  Rank mi = ((hi - lo) >> 1) + lo;
  mergeSort(lo, mi);
  mergeSort(mi, hi);

  T *A = _elem + lo, *B = new T[mi - lo], *C = _elem + mi;
  for (Rank i = 0; i < mi - lo; i++) {
    B[i] = A[i];
  }
  Rank i = 0, j = 0, k = 0;
  for (; j < mi - lo && k < hi - mi;) {
    if (B[j] < C[k]) {
      A[i++] = B[j++];
    } else {
      A[i++] = C[k++];
    }
  }
  if (k >= hi - mi) {
    while (j < mi - lo) {
      A[i++] = B[j++];
    }
  }
  delete[] B;
}

//
// 快速排序
//
template <typename T>
void MyVector<T>::quickSort(Rank lo, Rank hi) {
  if (hi - lo < 2) {
    return;
  }

  Rank mi = [this](Rank lo, Rank hi) -> Rank {
    Rank provt = this->_elem[lo];
    while (lo < hi - 1) {
      while (lo < hi - 1 && provt < this->_elem[hi - 1]) {
        hi--;
      }
      this->_elem[lo] = this->_elem[hi - 1];
      while (lo < hi - 1 && this->_elem[lo] <= provt) {
        lo++;
      }
      this->_elem[hi - 1] = this->_elem[lo];
    }
    this->_elem[lo] = provt;
    return lo;
  }(lo, hi);

  quickSort(lo, mi);
  quickSort(mi + 1, hi);
}

//
// 重载[]
//
template <typename T>
T& MyVector<T>::operator[](Rank r) {
  /* if (0 < r || _size < r - 1) {
    throw out_of_range("vector index out");
  } */
  return _elem[r];
}

//
// 重载=
//
template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& v) {
  if (this == &v) {
    return *this;
  }
  if (_elem) {
    delete[] _elem;
    _elem = nullptr;
  }
  copyFrom(v._elem, 0, v._size);
  return *this;
}

template <typename T>
Rank MyVector<T>::Size() {
  return _size;
}

template <typename T>
bool MyVector<T>::Empty() {
  return _size == 0;
}

//
// 插入操作 返回插入元素的秩
//
template <typename T>
Rank MyVector<T>::Insert(Rank r, const T& e) {
  expand();

  for (int i = _size; i > r; i--) {
    _elem[i] = _elem[i - 1];
  }
  _elem[r] = e;
  _size++;
  return r;
}

//
// 区间删除 删除[lo, hi)的所有元素 返回被删除元素的规模
//
template <typename T>
int MyVector<T>::Remove(Rank lo, Rank hi) {
  if (lo == hi) {
    return 0;
  }
  while (hi < _size) {
    _elem[lo] = _elem[hi];
    lo++, hi++;
  }
  _size = lo;
  shrink();
  return hi - lo;
}

//
// 删除秩为r的元素 返回被删除的元素
//
template <typename T>
T MyVector<T>::RemoveAt(Rank r) {
  T e = _elem[r];
  Remove(r, r + 1);
  return e;
}

//
// TODO
//
template <typename T>
Rank MyVector<T>::Find(const T& e, Rank lo, Rank hi) {
  while ((lo < hi--) && (e != _elem[hi]))
    ;
  return hi;
}

template <typename T>
template <typename VST>
void MyVector<T>::Traverse(VST& visit) {
  for (int i = 0; i < _size; i++) {
    visit(_elem[i]);
  }
}

//
// 有序向量的搜索 返回不大于e的最后一个元素的秩
// 在[lo, hi)搜索 返回[lo - 1, hi - 1]
//
template <typename T>
Rank MyVector<T>::Search(const T& e, Rank lo, Rank hi) {
  // 0: binSearchC()
  // 1: fibSearch()
  return binSearchC(e, lo, hi);
}

//
// 将区间[li, hi)中的元素排序为有序递增的
//
template <typename T>
void MyVector<T>::Sort(Rank lo, Rank hi) {
  // bubbleSort(lo, hi);
  // selectSort(lo, hi);
  // mergeSort(lo, hi);
  quickSort(lo, hi);
}

#endif