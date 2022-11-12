#include "testMyRedBlack.h"
#include "MyBinNode.hpp"
#include "MyRedBlack.hpp"
#include "MyVector.hpp"
#include <cstdlib>

static bool isRB = true;

void judge(BinNodePosi<int> x) {
  isRB = true;
  if (!isBlack(x)) {
    isRB = false;
  }
  post_order_t(x);
}

void post_order_t(BinNodePosi<int> x) {
  if (!isRB || !x) {
    return;
  }
  post_order_t(x->lChild);
  post_order_t(x->rChild);

  if (!isRB) {
    return;
  }
  if (!isBlack(x)) {
    isRB = isRB && (isBlack(x->lChild) && isBlack(x->rChild));
  }

  int r = (x->rChild) ? (x->rChild->height) : (0);
  int l = (x->lChild) ? (x->lChild->height) : (0);
  isRB = isRB && (r == l);
}

void testMyRedBlack() {
  cout << "My RedBlack Test" << endl;
  srand(time(0));
  auto f = [](int x) { cout << x << ", "; };
  MyVector<int> a;
  int b[] = {151, 122, 84, 137, 3,   182, 122, 52,  69,  155, 190, 25,  163,
             21,  60,  4,  16,  35,  77,  6,   160, 71,  13,  132, 190, 198,
             128, 40,  91, 195, 183, 118, 138, 89,  165, 66,  50,  57};

  MyRedBlack<int> mrb;
  for (int i = 0; i < rand() % 30 + 30 && isRB; i++) {
    int t = rand() % 300;
    // t = b[i];
    cout << "Insert " << t << endl;
    mrb.Insert(t);
    a.Insert(a.Size(), t);
    judge(mrb.Root());
    cout << "isRB " << isRB << endl;
  }
  a.Traverse(f);
  cout << endl;
  mrb.InOrderTraverse(mrb.Root(), f);
  cout << endl;
  for (int i = 0; isRB && !a.Empty(); i++) {
    int t = a[rand() % a.Size()];
    // t = 4;
    cout << "Remove " << t << endl;
    mrb.Remove(t);
    if (a.Find(t, 0, a.Size()) >= 0) {
      a.RemoveAt(a.Find(t, 0, a.Size()));
    }

    judge(mrb.Root());
    cout << "isRB " << isRB << endl;
    mrb.InOrderTraverse(mrb.Root(), f);
    cout << endl;
  }

}