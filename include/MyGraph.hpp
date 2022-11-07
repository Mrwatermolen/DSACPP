#ifndef MYGRAPH_HPP
#define MYGRAPH_HPP
#include <ctime>

#include "MyBitmap.h"
#include "MyQueue.hpp"

// 点的状态
typedef enum { UNDISCOVERED, DISCOVER, VISITED } VStatus;
// 边的类型
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EStatus;

template <typename Tv>
struct Vertex {
  Tv data;
  int inDegree, outDegree;  // 点的度
  VStatus status;           // 点状态
  int dTime, fTime;         // 时间标签
  int parent;               // 遍历中的父节点
  int priority;             // 优先级
  Vertex(const Tv& d)
      : data(d),
        inDegree(0),
        outDegree(0),
        status(UNDISCOVERED),
        dTime(-1),
        fTime(-1),
        parent(-1),
        priority(INT_MAX){};
  Vertex()
      : inDegree(0),
        outDegree(0),
        status(UNDISCOVERED),
        dTime(-1),
        fTime(-1),
        parent(-1),
        priority(INT_MAX){};
  Vertex<Tv>& operator=(const Vertex<Tv> v) {
    if (this == &v) {
      return *this;
    }
    data = v.data;
    status = v.status;
    inDegree = v.inDegree, outDegree = v.outDegree, dTime = v.dTime,
    fTime = v.fTime, parent = v.parent, priority = v.priority;
    return *this;
  };
};

template <typename Te>
struct Edge {
  Te data;
  int weight;      // 权重
  EStatus status;  // 边的类型
  Edge() : weight(0), status(UNDETERMINED){};
  Edge(const Te& d, int w) : data(d), weight(w), status(UNDETERMINED) {}
  Edge<Te>& operator=(const Edge<Te> e) {
    if (this == &e) {
      return *this;
    }
    data = e.data;
    weight = e.weight;
    status = e.status;
    return *this;
  };
};

template <typename Tv, typename Te>
class MyGraph {
 private:
  void reset();

 protected:
  int n;  // 点的数量
  int e;  // 边的数量

 public:
  virtual bool Exists(int i, int j) = 0;
  virtual int FirstNbr(int i) = 0;
  virtual int NextNbr(int i, int j) = 0;

  virtual Te& GetEdge(int i, int j) = 0;
  virtual EStatus& Status(int i, int j) = 0;
  virtual int& Weight(int i, int j) = 0;

  virtual Tv& GetVertex(int i) = 0;
  virtual VStatus& Status(int i) = 0;
  virtual int& InDegree(int i) = 0;
  virtual int& OutDegree(int i) = 0;
  virtual int& DTime(int i) = 0;
  virtual int& FTime(int i) = 0;
  virtual int& Parent(int i) = 0;
  virtual int& Priority(int i) = 0;

  virtual void Insert(const Te& edge, int w, int i, int j) = 0;
  virtual Te Remove(int i, int j) = 0;

  virtual int Insert(const Tv& vertex) = 0;
  virtual Tv Remove(int i) = 0;

  void BFS(int start);
  void bfs(int i, int& clock);

  void DFS(int start);
  void dfs(int i, int& clock);

  MyStack<int> TopologicalSort();

  void bbc(int v, int& clock, MyStack<int>& S, MyBitmap& B);
  MyBitmap BBC();

  template <typename PU>
  void PFS(int s, PU prioUpdater);
};

/// @brief Prim最小生成树的优先级策略
template <typename Tv, typename Te>
struct PrimPu {
  virtual void operator()(MyGraph<Tv, Te>* g, int uk, int v) {
    if (g->Status(v) != UNDISCOVERED) {
      return;
    }
    if (g->Priority(v) > g->Weight(uk, v)) {
      g->Priority(v) = g->Weight(uk, v);
      g->Parent(v) = uk;
    }
  }
};

/// @brief Dijkstra最短路径的优先级策略
template <typename Tv, typename Te>
struct DjikPu {
  virtual void operator()(MyGraph<Tv, Te>* g, int uk, int v) {
    if (g->Status(v) != UNDISCOVERED) {
      return;
    }
    if (g->Priority(v) > g->Priority(uk) + g->Weight(uk, v)) {
      g->Priority(v) = g->Priority(uk) + g->Weight(uk, v);
      g->Parent(v) = uk;
    }
  }
};

/// @brief 复位所有顶点和边的信息
template <typename Tv, typename Te>
void MyGraph<Tv, Te>::reset() {
  for (int i = 0; i < n; i++) {
    Status(i) = UNDISCOVERED;
    DTime(i) = -1;
    FTime(i) = -1;
    Parent(i) = -1;
    Priority(i) = INT_MAX;
    for (int j = 0; j < n; j++) {
      if (Exists(i, j)) {
        Status(i, j) = UNDETERMINED;
      }
    }
  }
}

/// @brief BFS-VISIT
/// @param i 顶点
/// @param clock 时刻
template <typename Tv, typename Te>
void MyGraph<Tv, Te>::bfs(int i, int& clock) {
  MyQueue<int> Q;
  Q.Enqueue(i);
  Status(i) = DISCOVER;

  while (!Q.Empty()) {
    int x = Q.Dequeue();
    // visit
    DTime(x) = ++clock;

    for (int w = FirstNbr(x); - 1 < w; w = NextNbr(x, w)) {
      if (Status(w) != UNDISCOVERED) {
        Status(x, w) = CROSS;  // 跨边
        continue;
      }
      Q.Enqueue(w);
      Status(w) = DISCOVER;
      Status(x, w) = TREE;  // 树边
      Parent(w) = x;
    }
    Status(x) = VISITED;
  }
}

/// @brief 获得图的BFS序列
/// @param start 以顶点start为起点
template <typename Tv, typename Te>
void MyGraph<Tv, Te>::BFS(int start) {
  reset();

  int clock = 0, v = start;
  while (true) {
    if (Status(v) == UNDISCOVERED) {
      bfs(v, clock);
    }
    v = (v + 1) % n;
    if (start == v) {
      break;
    }
  }
}

/// @brief DFS-VISIT
/// @param i 顶点
/// @param clock 时刻
template <typename Tv, typename Te>
void MyGraph<Tv, Te>::dfs(int i, int& clock) {
  Status(i) = DISCOVER;
  DTime(i) = ++clock;
  for (int w = FirstNbr(i); - 1 < w; w = NextNbr(i, w)) {
    switch (Status(w)) {
      case UNDISCOVERED: {
        Status(i, w) = TREE;
        Parent(w) = i;
        dfs(w, clock);
        break;
      }
      case DISCOVER: {
        Status(i, w) = BACKWARD;
        break;
      }
      case VISITED: {
        if (DTime(i) > DTime(w)) {
          Status(i, w) = FORWARD;
        } else {
          Status(i, w) = CROSS;
        }
        break;
      }
      default:
        break;
    }
  }
  Status(i) = VISITED;
  FTime(i) = ++clock;
}

/// @brief 获得图的DFS序列
/// @param start 以顶点start为起点
template <typename Tv, typename Te>
void MyGraph<Tv, Te>::DFS(int start) {
  reset();
  int clock = 0, v = start;
  while (true) {
    if (Status(v) == UNDISCOVERED) {
      dfs(v, clock);
    }
    v = (v + 1) % n;
    if (start == v) {
      break;
    }
  }
}

/// @brief 获得DAG图的拓扑排序栈
/// @return DAG图的拓扑排序栈 如果DAG图存在拓扑排序栈非空
template <typename Tv, typename Te>
MyStack<int> MyGraph<Tv, Te>::TopologicalSort() {
  MyStack<int> S;
  // 验证从DAG的任意顶点出发进行DFS 总能得到拓扑排序
  srand(time(0));
  bool isDag = [this](int start, MyStack<int>& S) {
    this->reset();
    // 定义DFS-VISIT
    auto f = [this](auto&& self, int i, int& clock, MyStack<int>& S) -> bool {
      this->Status(i) = DISCOVER;
      DTime(i) = ++clock;
      for (int w = this->FirstNbr(i); - 1 < w; w = this->NextNbr(i, w)) {
        switch (this->Status(w)) {
          case UNDISCOVERED: {
            this->Status(i, w) = TREE;
            this->Parent(w) = i;
            if (self(self, w, clock, S)) {
              break;
            } else {
              return false;
            }
          }
          case DISCOVER: {
            // 出现后向边 有环 非DAG 结束
            this->Status(i, w) = BACKWARD;
            return false;
          }
          case VISITED: {
            // 无向图中不可能出现这种情况
            if (this->DTime(i) > this->DTime(w)) {
              this->Status(i, w) = FORWARD;
            } else {
              this->Status(i, w) = CROSS;
            }
            break;
          }
          default:
            break;
        }
      }
      Status(i) = VISITED;
      FTime(i) = ++clock;
      // 至此以顶点i为起点的有向边的已经访问结束 顶点i进入拓扑序列栈中
      S.Push(i);
      return true;
    };

    // 开始DFS
    int clock = 0, v = start;
    bool dag = false;
    while (true) {
      if (this->Status(v) == UNDISCOVERED) {
        dag = f(f, v, clock, S);
      }
      v = (v + 1) % n;
      if (!dag) {
        return dag;
      }
      if (start == v) {
        break;
      }
    }
    return dag;
  }((rand() % n), S);

  if (!isDag) {
    while (!S.Empty()) {
      S.Pop();
    }
  }
  return S;
}

template <typename Tv, typename Te>
void MyGraph<Tv, Te>::bbc(int v, int& clock, MyStack<int>& S, MyBitmap& B) {
  // hca >= dTime
  FTime(v) = DTime(v) = ++clock;
  Status(v) = DISCOVER;
  S.Push(v);
  for (int w = FirstNbr(v); - 1 < w; w = NextNbr(v, w)) {
    switch (Status(w)) {
      case UNDISCOVERED: {
        Parent(w) = v;
        Status(v, w) = TREE;
        bbc(w, clock, S, B);
        if (FTime(w) < DTime(v)) {
          // 子节点的hca小于自己dTime说明
          // 节点v也能通一条路径到达子节点w的hca
          // 于是更新节点c的hca
          FTime(v) = min(FTime(v), FTime(w));
        } else {
          // 子节点的hca大于于自己dTime说明
          // 子节点w只能从节点v访问到
          // 节点v必然为关节点
          while (true) {
            cout << GetVertex(S.Top());
            if (w == S.Pop()) {
              break;
            }
          }
          cout << GetVertex(S.Top()) << endl;
          B.Set(v);
        }
        break;
      }
      case DISCOVER: {
        // 发现了后向边 更新hca
        // if (u != Parent(w));
        FTime(v) = min(FTime(v), DTime(w));
        break;
      }
      default: {
        // 无向图中不可能出现
        if (DTime(v) > DTime(w)) {
          Status(v, w) = FORWARD;
        } else {
          Status(v, w) = CROSS;
        }
        break;
      }
    }
  }
}

/// @brief 给出无向图的 双连通分量 TODO:Refoctor
/// @return
template <typename Tv, typename Te>
MyBitmap MyGraph<Tv, Te>::BBC() {
  MyBitmap B(n);
  srand(time(0));
  int clock = 0;
  MyStack<int> S;
  bbc(rand() % n, clock, S, B);
  return B;
}

/// @brief 优先级搜索框架
/// @tparam PU
/// @param s
/// @param prioUpdater
template <typename Tv, typename Te>
template <typename PU>
void MyGraph<Tv, Te>::PFS(int s, PU prioUpdater) {
  reset();
  Priority(s) = 0;
  Parent(s) = -1;
  Status(s) = DISCOVER;
  while (true) {
    for (int w = FirstNbr(s); - 1 < w; w = NextNbr(s, w)) {
      prioUpdater(this, s, w);
    }
    for (int shortest = INT_MAX, w = 0; w < n; w++) {
      if (Status(w) == UNDISCOVERED && shortest > Priority(w)) {
        shortest = Priority(w);
        s = w;
      }
    }
    if (Status(s) == VISITED) {
      // 所有的节点都已经加入
      break;
    }
    Status(s) = VISITED;
    if (Parent(s) != -1) {
      Status(Parent(s), s) = TREE;
    }
  }
}

#endif