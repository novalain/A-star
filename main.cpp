#include <iostream>
#include <queue>

int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY,
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize);

struct Node {
  int x;
  int y;
  int id;
  int f;
};

struct Compare {
  bool operator()(const Node& left, const Node& right) {
    return left.f > right.f;
  }
};

// (x, y) -> index
int id(const int& x, const int& y, const int& mapWidth) {
  return y * mapWidth + x;
}

void Backtrack(int* pOutBuffer, std::vector<int>& predecessors, int& id, int endId, const int& nOutBufferSize) {
  int i = 0;
  int currentIndex = id;
  while (currentIndex != endId) {
    pOutBuffer[i] = currentIndex;
    currentIndex = predecessors[currentIndex];
    i++;
  }
}

// Brute force BFS
int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY,
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize) {

  std::priority_queue<Node, std::vector<Node>, Compare> queue;
  std::vector<int> visited(12);
  std::vector<int> predecessors(12, -1);

  Node start = {nStartX, nStartY, id(nStartX, nStartY, nMapWidth), 0};
  queue.push(start);

  while (!queue.empty()) {
    Node node = queue.top();
    queue.pop();

    std::cout << "Investigating node with ID " << node.id << std::endl;
    //std::cout << "This node predecessor " << predecessors[node.id] << std::endl;
    if (node.x == nTargetX && node.y == nTargetY) {
      if (node.f > nOutBufferSize) {
        return -1;
      }
      Backtrack(pOutBuffer, predecessors, node.id, id(nStartX, nStartY, nMapWidth), nOutBufferSize);
      return node.f;
    }
    visited[node.id] = 1;

    // X-dir
    for (int i = -1; i < 2; i += 2) {
      Node neighbor = {node.x + i, node.y, id(node.x + i, node.y, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.x <= nMapWidth && pMap[neighbor.id] == 1) {
        neighbor.f = node.f + 1;
        predecessors[neighbor.id] = node.id;
        queue.push(neighbor);
      }
    }

    // Y-dir
    for (int i = -1; i < 2; i += 2) {
      Node neighbor = {node.x, node.y + i, id(node.x, node.y + i, nMapWidth), 0};
      if (!visited[neighbor.id]  && neighbor.y <= nMapHeight && pMap[neighbor.id] == 1) {
        neighbor.f = node.f + 1;
        predecessors[neighbor.id] = node.id;
        queue.push(neighbor);
      }
    }
  }
  // Fail
  return -1;
}

int main() {

  // Test case 1
  //const unsigned char pMap[] = {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
  //int pOutBuffer[12];
  //int a = FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12);
  //Expected output: 3 and {1, 5, 9}

  // Test case 2
  unsigned char pMap[] = {0, 0, 1, 0, 1, 1, 1, 0, 1};
  int pOutBuffer[7];
  int a = FindPath(2, 0, 0, 2, pMap, 3, 3, pOutBuffer, 7);
  //Expected output: -1

  std::cout << "Length shortest path: " << a << std::endl;

  std::cout << "{";
  for (int i = 0; i < a; i++) {
     std::cout << pOutBuffer[i];
     if (i != a - 1 ) {std::cout << ", ";}
  }
  std::cout << "}\n";
  return 0;
}
//   F
// 0 1 1 1
// 0 1 0 1
// 1 1 1 1
// S

// (0, 1) -> 4, (1, 1) -> 5, (2, 1) -> 6, (3, 1) -> 7
// (0, 0) -> 0, (1, 0) -> 1, (2, 0) -> 2, (3, 0) -> 3

// (0, 1) -> 3, (1, 1) -> 4, (2, 1) -> 5
// (0, 0) -> 0, (1, 0) -> 1, (2, 0) -> 2

// id = y * mapWidth + x
