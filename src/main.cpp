#include <cstdlib>
#include <iostream>
#include <iterator>
#include <random>
#include <string>

#include "MyAVL.hpp"
#include "MyBST.hpp"
#include "MyBTree.hpp"
#include "MyBinTree.hpp"
#include "MyBitmap.h"
#include "MyGraph.hpp"
#include "MyGraphMatrix.hpp"
#include "MyList.hpp"
#include "MyQueue.hpp"
#include "MySplay.hpp"
#include "MyStack.hpp"
#include "test.h"
#include "testMyHuffmanCoder.h"
#include "testMyPQ.h"
#include "testMyRedBlack.h"

using namespace std;

void testMyBitmap() {
  if (!MyBitmapDebug) {
    return;
  }
  cout << "My Bitmap Test" << endl;
  int n = 100;
  MyBitmap b(n);
  b.Set(0), b.Set(1);
  for (int i = 2; i < n; i++) {
    if (!b.Test(i)) {
      for (int j = 2 * i; j < n; j += i) {
        b.Set(j);
      }
    }
  }
  for (int i = 2; i < n; i++) {
    if (!b.Test(i)) {
      cout << i << endl;
    }
  }
}

void testMyList() {
  cout << "My List Test" << endl;
  MyList<int> ml;
  auto f = [](const int x) { cout << x << "->"; };
  auto p = ml.InsertAsFirst(1);
  auto q = ml.InsertAfter(p, 3);
  ml.InsertBefore(q, 2);
  ml.InsertAsLast(4);
  ml.Traverse(f);
  cout << endl;
}

void testMyStack() {
  cout << "My Stack Test" << endl;
  MyStack<int> ms;
  ms.Push(3);
  ms.Push(4);
  ms.Push(2);
  ms.Push(1);
  while (!ms.Empty()) {
    cout << ms.Top();
    ms.Pop();
  }
  cout << endl;
}

void testMyQueue() {
  cout << "My Queue Test" << endl;
  MyQueue<int> mq;
  mq.Enqueue(4);
  mq.Enqueue(3);
  mq.Enqueue(2);
  cout << mq.Front() << endl;
  mq.Enqueue(1);
  while (!mq.Empty()) {
    cout << mq.Front();
    mq.Dequeue();
  }
  cout << endl;
}

void testMyBinTree() {
  cout << "My BinTree Test" << endl;
  MyBinTree<int> mt(12);
  mt.InsertAsLC(7, mt.Root())->InsertAsLC(6)->InsertAsLC(2)->InsertAsLC(1);
  auto p = mt.InsertAsRC(14, mt.Root());
  p->InsertAsLC(13);
  p->InsertAsRC(15);
  mt.Root()->lChild->lChild->lChild->InsertAsRC(5)->InsertAsLC(3)->InsertAsRC(
      4);
  p = mt.Root()->lChild->InsertAsRC(11)->InsertAsLC(9);
  p->InsertAsLC(8);
  p->InsertAsRC(10);
  auto f = [](int x) { cout << x << " "; };
  mt.PreOrderTraverse(mt.Root(), f);
  cout << endl;
  mt.InOrderTraverse(mt.Root(), f);
  cout << endl;
  mt.PostOrderTraverse(mt.Root(), f);
  cout << endl;
  mt.LevelOrderTraverse(mt.Root(), f);
  cout << endl;
}

void testMyGraph() {
  cout << "My Graph Test" << endl;
  // ??????DAG??? ???????????????<v7->v4>???????????????????????????
  string ver[] = {"v0", "v1", "v2", "v3",  "v4",  "v5", "v6",
                  "v7", "v8", "v9", "v10", "v11", "v12"};
  int edge1[] = {0, 0, 0, 0, 0, 0, 1, 2, 3, 3,  3,  4,
                 4, 5, 5, 6, 7, 8, 8, 8, 9, 10, 11, 11};
  int edge2[] = {1, 2, 3,  4, 5, 6, 7,  7,  7,  9,  11, 8,
                 9, 8, 12, 8, 9, 7, 10, 11, 12, 11, 9,  12};
  int info[] = {1,  6,  3,  4, 3, 1, 6, 2, 7,  8, 6, 5,
                11, 10, 21, 6, 8, 4, 9, 4, 10, 9, 3, 12};
  MyVector<int> verNum;
  MyGraphMatrix<string, int> mgm_1;
  for (int i = 0; i < 13; i++) {
    verNum.Insert(verNum.Size(), mgm_1.Insert(ver[i]));
  }
  for (int i = 0; i < 24; i++) {
    mgm_1.Insert(info[i], info[i], verNum[edge1[i]], verNum[edge2[i]]);
  }
  auto s = mgm_1.TopologicalSort();
  while (!s.Empty()) {
    cout << mgm_1.GetVertex(s.Pop()) << " ";
  }
  cout << endl;
  mgm_1.PFS(0, DjikPu<string, int>());
  cout << "DJL " << endl;
  for (int i = 0; i < 13; i++) {
    int p = i, dist = 0;
    while (-1 < p) {
      dist += mgm_1.Priority(p);
      cout << mgm_1.GetVertex(p) << "<-";
      p = mgm_1.Parent(p);
    }
    cout << "  dist " << dist << endl;
  }
  mgm_1.PFS(0, PrimPu<string, int>());
  int cost = 0;
  for (int i = 0; i < 13; i++) {
    cost += mgm_1.Priority(i);
  }
  cout << "Prim MIT cost " << cost << endl;

  // ???????????????
  string ver1[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};
  int edge11[] = {0, 0, 1, 2, 2, 2, 3, 4, 4, 5, 5, 5, 5, 6, 9, 10};
  int edge21[] = {1, 4, 5, 3, 5, 7, 7, 5, 8, 6, 8, 9, 10, 10, 10, 11};
  MyGraphMatrix<string, int> mgm_2;
  for (int i = 0; i < 12; i++) {
    verNum.Insert(verNum.Size(), mgm_2.Insert(ver1[i]));
  }
  for (int i = 0; i < 16; i++) {
    mgm_2.Insert(info[i], info[i], verNum[edge11[i]], verNum[edge21[i]]);
    mgm_2.Insert(info[i], info[i], verNum[edge21[i]], verNum[edge11[i]]);
  }
  mgm_2.BBC();
}

void testMyBST() {
  cout << "My BST Test" << endl;
  auto f = [](int x) { cout << x << ", "; };
  MyBST<int> mbst(4);
  for (int i = 0; i < 9; i++) {
    mbst.Insert(i);
  }

  mbst.InOrderTraverse(mbst.Root(), f);
  cout << endl;
  mbst.PreOrderTraverse(mbst.Root(), f);
  cout << endl;
  srand(time(0));
  for (int i = 0; i < 3; i++) {
    int t = rand() % 9;
    cout << "remove " << t << endl;
    mbst.Remove(t);
    mbst.InOrderTraverse(mbst.Root(), f);
    cout << endl;
  }
  mbst.InOrderTraverse(mbst.Root(), f);
  cout << endl;
  mbst.PreOrderTraverse(mbst.Root(), f);
  cout << endl;
  cout << "My AVL Test" << endl;
  MyAVL<int> mavl(4);
  for (int i = 0; i < 9; i++) {
    mavl.Insert(i);
  }
  mavl.InOrderTraverse(mavl.Root(), f);
  cout << endl;
  mavl.PreOrderTraverse(mavl.Root(), f);
  for (int i = 0; i < 3; i++) {
    int t = rand() % 9;
    cout << "remove " << t << endl;
    mavl.Remove(t);
    mavl.InOrderTraverse(mbst.Root(), f);
    cout << endl;
  }
  mavl.InOrderTraverse(mavl.Root(), f);
  cout << endl;
  mavl.PreOrderTraverse(mavl.Root(), f);
  cout << endl;
  cout << "My Splay Test" << endl;
  MySlpay<int> ms(0);
  for (int i = 1; i < 10; i++) {
    ms.Insert(i);
  }
  ms.InOrderTraverse(ms.Root(), f);
  cout << endl;
  ms.PreOrderTraverse(ms.Root(), f);
  cout << endl;
  for (int i = 0; i < 3; i++) {
    int t = rand() % 9;
    ms.Search(t);
    cout << "Search " << t << endl;
    ms.InOrderTraverse(ms.Root(), f);
    cout << endl;
    ms.PreOrderTraverse(ms.Root(), f);
    cout << endl;
  }
  for (int i = 0; i < 4; i++) {
    int t = rand() % 10;
    ms.Remove(t);
    cout << "Remove " << t << endl;
    ms.InOrderTraverse(ms.Root(), f);
    cout << endl;
    ms.PreOrderTraverse(ms.Root(), f);
    cout << endl;
  }
}

void testMyBTree() {
  cout << "My B Test" << endl;
  auto f = [](int x) { cout << x << " "; };
  MyBTree<int> mb(5, rand() % 100);
  srand(time(0));
  MyVector<int> a(20);
  for (int i = 0; i < 20; i++) {
    int t = rand() % 200;
    cout << "Insert " << t << endl;
    mb.Insert(t);
    a.Insert(0, t);
    mb.LevelOrderTraverse(mb.Root(), f);
    mb.InOrderTraverse(mb.Root(), f);
    cout << endl;
  }
  for (int i = 0; i < 5; i++) {
    int t = rand() % a.Size();
    cout << "Remove " << a[t] << endl;
    mb.Remove(a[t]);
    mb.LevelOrderTraverse(mb.Root(), f);
    mb.InOrderTraverse(mb.Root(), f);
    a.RemoveAt(t);
    cout << endl;
  }
}

int main() {
  // testMyVector();
  // testMyBitmap();
  // testMyList();
  // testMyStack();
  // testMyQueue();
  // testMyBinTree();
  // testMyGraph();
  // testMyBST();
  // testMyBTree();
  // testMyRedBlack();
  // testMyPQ();
  testMyHuffmanCoder();
}