#ifndef MYGRAPHMATRIX_HPP
#define MYGRAPHMATRIX_HPP
#include "MyGraph.hpp"
#include "MyVector.hpp"

template <typename Tv, typename Te>
class MyGraphMatrix : public MyGraph<Tv, Te> {
 private:
  MyVector<Vertex<Tv>> V;  // 顶点集

  MyVector<MyVector<Edge<Te>*>> E;  // 边集
 public:
  MyGraphMatrix() { this->n = this->e = 0; };
  ~MyGraphMatrix() {
    for (int i = 0; i < this->n; i++) {
      for (int j = 0; j < this->n; j++) {
        delete E[i][j];
      }
    }
  };

  bool Exists(int i, int j);
  int FirstNbr(int i);
  int NextNbr(int i, int j);

  Te& GetEdge(int i, int j);
  EStatus& Status(int i, int j);
  int& Weight(int i, int j);
  void Insert(const Te& edge, int w, int i, int j);
  Te Remove(int i, int j);

  Tv& GetVertex(int i);
  VStatus& Status(int i);
  int& InDegree(int i);
  int& OutDegree(int i);
  int& DTime(int i);
  int& FTime(int i);
  int& Parent(int i);
  int& Priority(int i);

  int Insert(const Tv& vertex);
  Tv Remove(int i);
};

//
// 判断边<i, j>是否存在
//
template <typename Tv, typename Te>
bool MyGraphMatrix<Tv, Te>::Exists(int i, int j) {
  return ((0 <= i && i < this->n) && (0 <= j && j < this->n) && (E[i][j]));
}

//
// 返回与顶点i邻接的第一个点 不存在返回-1
//
template <typename Tv, typename Te>
int MyGraphMatrix<Tv, Te>::FirstNbr(int i) {
  return NextNbr(i, this->n);
}

template <typename Tv, typename Te>
int MyGraphMatrix<Tv, Te>::NextNbr(int i, int j) {
  while ((-1 < j) && !Exists(i, --j))
    ;
  return j;
}

//
// 返回边<i, j>的数据
// 不做检查
//
template <typename Tv, typename Te>
Te& MyGraphMatrix<Tv, Te>::GetEdge(int i, int j) {
  return E[i][j]->data;
}

//
// 返回边<i, j>的状态
// 不做检查
//
template <typename Tv, typename Te>
EStatus& MyGraphMatrix<Tv, Te>::Status(int i, int j) {
  return E[i][j]->status;
}

//
// 返回边<i, j>的权重
// 不做检查
//
template <typename Tv, typename Te>
int& MyGraphMatrix<Tv, Te>::Weight(int i, int j) {
  return E[i][j]->weight;
}

//
// 插入边<i, j>
//
template <typename Tv, typename Te>
void MyGraphMatrix<Tv, Te>::Insert(const Te& edge, int w, int i, int j) {
  if (Exists(i, j)) {
    return;
  }
  E[i][j] = new Edge<Te>(edge, w);
  this->e++;
  V[i].outDegree++;
  V[j].inDegree++;
}

//
// 删除边<i, j> 返回删除边的信息
// 不做检查
//
template <typename Tv, typename Te>
Te MyGraphMatrix<Tv, Te>::Remove(int i, int j) {
  Te eB = GetEdge(i, j);
  delete E[i][j];
  E[i][j] = nullptr;
  this->e--;
  V[i].outDegree--;
  V[j].inDegree--;
  return eB;
}

//
// 获得顶点i的数据
// 不做检查
//
template <typename Tv, typename Te>
Tv& MyGraphMatrix<Tv, Te>::GetVertex(int i) {
  return V[i].data;
}

//
// 获得顶点i的状态
// 不做检查
//
template <typename Tv, typename Te>
VStatus& MyGraphMatrix<Tv, Te>::Status(int i) {
  return V[i].status;
}

//
// 获得顶点入度
// 不做检查
//
template <typename Tv, typename Te>
int& MyGraphMatrix<Tv, Te>::InDegree(int i) {
  return V[i].inDegree;
}

//
// 获得顶点出度
// 不做检查
//
template <typename Tv, typename Te>
int& MyGraphMatrix<Tv, Te>::OutDegree(int i) {
  return V[i].outDegree;
}

//
// 获得
// 不做检查
//
template <typename Tv, typename Te>
int& MyGraphMatrix<Tv, Te>::DTime(int i) {
  return V[i].dTime;
}

//
// 获得
// 不做检查
//
template <typename Tv, typename Te>
int& MyGraphMatrix<Tv, Te>::FTime(int i) {
  return V[i].fTime;
}

//
// 获得
// 不做检查
//
template <typename Tv, typename Te>
int& MyGraphMatrix<Tv, Te>::Parent(int i) {
  return V[i].parent;
}

//
// 获得
// 不做检查
//
template <typename Tv, typename Te>
int& MyGraphMatrix<Tv, Te>::Priority(int i) {
  return V[i].priority;
}

//
// 插入顶点 返回顶点编号
//
template <typename Tv, typename Te>
int MyGraphMatrix<Tv, Te>::Insert(const Tv& vertex) {
  for (int i = 0; i < this->n; i++) {
    E[i].Insert(E[i].Size(), nullptr);
  }
  this->n++;
  auto p = MyVector<Edge<Te>*>(this->n, nullptr);
  E.Insert(E.Size(), p);
  return V.Insert(V.Size(), Vertex<Tv>(vertex));
}

//
// 删除顶点 返回顶点
// 不做检查
//
template <typename Tv, typename Te>
Tv MyGraphMatrix<Tv, Te>::Remove(int i) {
  Tv vB = GetVertex(i);
  // 清空行信息
  for (int j = 0; j < this->n; j++) {
    if (!Exists(i, j)) {
      continue;
    }
    Remove(i, j);
  }
  // 清空列信息
  for (int j = 0; j < this->n; j++) {
    if (!Exists(j, i)) {
      continue;
    }
    this->e--;
    E[j].RemoveAt(i);
    V[j].outDegree--;
  }
  E.RemoveAt(i);  // 删除
  V.RemoveAt(i);
  this->n--;
  return vB;
}

#endif