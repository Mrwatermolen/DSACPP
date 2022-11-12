#include "testMyVector.h"

void testMyVector() {
  cout << "My Vector Test" << endl;
  int a[] = {3, 0, 8, 2, 2, 4, 7, 5, 1}, l = 0, h = 9;
  MyVector<int> myV;
  for (int i = 8; i >= 0; i--) {
    myV.Insert(0, a[i]);
  }
  auto f = [](int a) { cout << a; };
  myV.Traverse(f);
  cout << endl;
  myV.Sort(0, 9);
  myV.Traverse(f);
  cout << endl;
  cout << myV.Search(7, 0, 9) << endl;
  myV.RemoveAt(3);
  myV.RemoveAt(3);
  myV.Traverse(f);
  cout << endl;
  cout << myV.Search(3, 0, myV.Size());
  cout << endl;
  cout << myV.Insert(myV.Search(3, 0, myV.Size()) + 1, 3);
  cout << endl;
  myV.Traverse(f);
  cout << endl;
}