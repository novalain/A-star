/**
  Path-finding
  Michael Novén
  20/03/17
*/

#include <queue>

#include <iostream>
#include <chrono>

struct Node {
  int x;
  int y;
  int id;
  int f;
  int g;
};

struct CompareNodes {
  bool operator()(const Node& left, const Node& right) {
    return left.f > right.f;
  }
};

// (x, y) -> index [0, w * h - 1]
int ComputeId(const int& x, const int& y, const int& mapWidth) {
  return y * mapWidth + x;
}

void Backtrack(int* pOutBuffer, const std::vector<int>& predecessors, const int& startId,
               const int& endId, const int& pathLength, const int& nOutBufferSize) {
  int currentIndex = endId;
  int i = 0;
  while (currentIndex != startId) {
    pOutBuffer[pathLength - 1 - i++] = currentIndex;
    currentIndex = predecessors[currentIndex];
  }
}

int Manhattan(const int& x, const int& y, const int& tx, const int& ty) {
  return std::max(x - tx, tx - x) + std::max(y - ty, ty - y);
}

int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY,
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize) {

  std::priority_queue<Node, std::vector<Node>, CompareNodes> queue;
  std::vector<int> visited(nMapWidth * nMapHeight);
  std::vector<int> predecessors(nMapWidth * nMapHeight, -1);

  const int startId = ComputeId(nStartX, nStartY, nMapWidth);
  Node start = {nStartX, nStartY, startId, 0};
  queue.push(start);

  while (!queue.empty()) {
    Node node = queue.top();
    queue.pop();

    // Buffer exceeded, call again
    if (node.g > nOutBufferSize) {
      int nOutBufferSizeNew = nOutBufferSize != 0 ? nOutBufferSize * 2 : 10;
      pOutBuffer[nOutBufferSizeNew];
      return FindPath(nStartX, nStartY, nTargetX, nTargetY, pMap, nMapWidth, nMapHeight,
               pOutBuffer, nOutBufferSizeNew);
    }
    // All done!
    if ((node.x == nTargetX && node.y == nTargetY)) {
      Backtrack(pOutBuffer, predecessors, startId, node.id, node.g, nOutBufferSize);
      return node.g;
    }
    // Slower path, no need to visit again
    if (visited[node.id]) {
      continue;
    }
    visited[node.id] = 1;

    // Right
    {
      int neighborX = node.x + 1;
      Node neighbor = {neighborX, node.y, ComputeId(neighborX, node.y, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.x < nMapWidth && pMap[neighbor.id] != 0) {
          int distance = node.g + 1;
          neighbor.g = distance;
          neighbor.f = distance + Manhattan(neighborX, node.y, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
      }
    }
    // Left
    {
      int neighborX = node.x - 1;
      Node neighbor = {neighborX, node.y, ComputeId(neighborX, node.y, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.x >= 0 && pMap[neighbor.id] != 0) {
          int distance = node.g + 1;
          neighbor.g = distance;
          neighbor.f = distance + Manhattan(neighborX, node.y, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
      }
    }
    // Top
    {
      int neighborY = node.y + 1;
      Node neighbor = {node.x, neighborY, ComputeId(node.x, neighborY, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.y < nMapHeight && pMap[neighbor.id] != 0) {
          int distance = node.g + 1;
          neighbor.g = distance;
          neighbor.f = distance + Manhattan(node.x, neighborY, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
      }
    }
    // Bottom
    {
      int neighborY = node.y - 1;
      Node neighbor = {node.x, neighborY, ComputeId(node.x, neighborY, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.y >= 0 && pMap[neighbor.id] != 0) {
        int distance = node.g + 1;
        neighbor.g = distance;
        neighbor.f = distance + Manhattan(node.x, neighborY, nTargetX, nTargetY);
        predecessors[neighbor.id] = node.id;
        queue.push(neighbor);
      }
    }
  }
  // Fail
  return -1;
}

int main() {

  // // Test case 1
  const unsigned char pMap[] = {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
  const int startX = 0, startY = 0, endX = 1, endY = 2, mapWidth = 4, mapHeight = 3;
  int pOutBufferSize = 0;
  int pOutBuffer[0];

 // Test case 2
  // unsigned char pMap[] = {0, 0, 1, 0, 1, 1, 1, 0, 1};
  // int pOutBuffer[7];
  // int a = FindPath(2, 0, 0, 2, pMap, 3, 3, pOutBuffer, 7);
  // Expected output: -1

  // 1 1 1 1
  // 0 1 0 1
  // 1 1 1 1

  //  Special case
  // const unsigned char pMap[] = {1, 1, 1, 1, 1, 1, 1};
  // const int startX = 0, startY = 0, endX = 0, endY = 6, mapWidth = 2, mapHeight = 7;
  // int pOutBufferSize = 5;
  // int pOutBuffer[5];

  // 1
  // 1
  // 1
  // 1
  // 0
  // 1
  // 1
  // Custom
  // unsigned const int mapWidth = 512;
  // unsigned const int mapHeight = 512;
  // unsigned char pMap[mapWidth * mapHeight];
  // fetchData(pMap, "testcases/swampofsorrows.map");
  // int pOutBuffer[mapWidth * mapHeight];
  // int pOutBufferSize = mapWidth * mapHeight;
  // int startX = 2, startY = 2, endX = 510, endY = 510;

  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  int a = FindPath(startX, startY, endX, endY, pMap, mapWidth, mapHeight, pOutBuffer, pOutBufferSize);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

  std::cout << "Time taken: " << duration << "s" << std::endl;
  //std::cout << "Pops from queue: " << popsFromQueue << std::endl;
  std::cout << "Length shortest path: " << a << std::endl;
  std::cout << "{";
  for (int i = 0; i < a; i++) {
     std::cout << pOutBuffer[i];
     if (i != a - 1 ) {std::cout << ", ";}
  }
  std::cout << "}\n";
  exit(0);
}
