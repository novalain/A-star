/**
  Path-finding
  Michael Novén
  20/03/17
*/

#include <queue>
#include <cmath>
// #include <chrono>
// #include <iostream>

struct Node {
  int x;
  int y;
  int id;
  int f;
};

bool operator<(const Node& left, const Node& right) {
  return left.f > right.f;
}

// (x, y) -> index [0, w * h - 1]
int ComputeId(const int& x, const int& y, const int& mapWidth) {
  return y * mapWidth + x;
}

void Backtrack(int* pOutBuffer, const std::vector<int>& predecessors,
               const int& startId, const int& endId, const int& pathLength,
               const int& nOutBufferSize) {
  int currentIndex = endId;
  int i = 0;
  while (currentIndex != startId) {
    pOutBuffer[pathLength - 1 - i++] = currentIndex;
    currentIndex = predecessors[currentIndex];
  }
}

int Manhattan(const int& x, const int& y, const int& tx, const int& ty) {
  return std::abs(x - tx) + std::abs(y - ty);
}

int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY,
             const unsigned char* pMap, const int nMapWidth,
             const int nMapHeight, int* pOutBuffer, const int nOutBufferSize) {
  std::priority_queue<Node> queue;
  std::vector<int> visited(nMapWidth * nMapHeight);
  std::vector<int> predecessors(nMapWidth * nMapHeight, -1);
  std::vector<int> distances(nMapWidth * nMapHeight, -1);

  const int startId = ComputeId(nStartX, nStartY, nMapWidth);
  const int targetId = ComputeId(nTargetX, nTargetY, nMapWidth);
  Node start = {nStartX, nStartY, startId, 0};
  distances[start.id] = 0;
  queue.push(start);

  while (!queue.empty()) {
    Node node = queue.top();
    queue.pop();
    int currentDist = distances[node.id];

    // All done!
    if (node.id == targetId) {
      // Buffer exceeded, skip writing and just return length
      if (currentDist <= nOutBufferSize) {
        Backtrack(pOutBuffer, predecessors, startId, node.id, distances[node.id],
                nOutBufferSize);
      }
      return currentDist;
    }

    // For all neighbors..
    // TODO: Do these 4 cases prettier without making unnecessary checks
    // Right
    {
      int neighborX = node.x + 1;
      int neighborId = ComputeId(neighborX, node.y, nMapWidth);
      int currentDistNeighbor = distances[neighborId];
      // Boundaries
      if (neighborX < nMapWidth && pMap[neighborId]) {
        // If not visited or better path
        if (currentDistNeighbor == -1 || (currentDist + 1 < currentDistNeighbor)) {
          Node neighbor = {neighborX, node.y, neighborId, 0};
          distances[neighbor.id] = currentDist + 1;
          neighbor.f = currentDist + 1 +
                       Manhattan(neighbor.x, neighbor.y, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
        }
      }
    }
    // Left
    {
      int neighborX = node.x - 1;
      int neighborId = ComputeId(neighborX, node.y, nMapWidth);
      int currentDistNeighbor = distances[neighborId];
      // Boundaries
      if (neighborX >= 0 && pMap[neighborId]) {
        // If not visited or better path
        if (currentDistNeighbor == -1 || (currentDist + 1 < currentDistNeighbor)) {
          Node neighbor = {neighborX, node.y, neighborId, 0};
          distances[neighbor.id] = currentDist + 1;
          neighbor.f = distances[node.id] + 1 +
                         Manhattan(neighbor.x, neighbor.y, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
        }
      }
    }
    // Top
    {
      int neighborY = node.y + 1;
      int neighborId = ComputeId(node.x, neighborY, nMapWidth);
      int currentDistNeighbor = distances[neighborId];
      // Boundaries
      if (neighborY < nMapHeight && pMap[neighborId]) {
        // If not visited or better path
        if (distances[neighborId] == -1 || (currentDist + 1) < currentDistNeighbor) {
          Node neighbor = {node.x, neighborY, neighborId, 0};
          distances[neighbor.id] = currentDist + 1;
          neighbor.f = currentDist + 1 +
                         Manhattan(neighbor.x, neighbor.y, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
        }
      }
    }
    // Bottom
    {
      int neighborY = node.y - 1;
      int neighborId = ComputeId(node.x, neighborY, nMapWidth);
      int currentDistNeighbor = distances[neighborId];
      // Boundaries
      if (neighborY >= 0 && pMap[neighborId]) {
        // If not visited or better path
        if (currentDistNeighbor == -1 || (currentDist + 1 < currentDistNeighbor)) {
          Node neighbor = {node.x, neighborY, neighborId, 0};
          distances[neighbor.id] = currentDist + 1;
          neighbor.f = currentDist + 1 +
                         Manhattan(neighbor.x, neighbor.y, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
        }
      }
    }
  }
  // Fail
  return -1;
}

// int main() {
//   // // Test case 1
//   const unsigned char pMap[] = {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
//   const int startX = 0, startY = 0, endX = 1, endY = 2, mapWidth = 4, mapHeight = 3;
//   int pOutBufferSize = 5;
//   int pOutBuffer[5];

//  // Test case 2
//   // unsigned char pMap[] = {0, 0, 1, 0, 1, 1, 1, 0, 1};
//   // int pOutBuffer[7];
//   // int a = FindPath(2, 0, 0, 2, pMap, 3, 3, pOutBuffer, 7);
//   // Expected output: -1

//   // 1 1 1 1
//   // 0 1 0 1
//   // 1 1 1 1

//   // Custom
//   // unsigned const int mapWidth = 512;
//   // unsigned const int mapHeight = 512;
//   // unsigned char pMap[mapWidth * mapHeight];
//   // fetchData(pMap, "testcases/swampofsorrows.map");
//   // int pOutBuffer[mapWidth * mapHeight];
//   // int pOutBufferSize = mapWidth * mapHeight;
//   // int startX = 2, startY = 2, endX = 510, endY = 510;

//   std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
//   int a = FindPath(startX, startY, endX, endY, pMap, mapWidth, mapHeight, pOutBuffer, pOutBufferSize);
//   std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
//   auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

//   std::cout << "Time taken: " << duration << "s" << std::endl;
//   //std::cout << "Pops from queue: " << popsFromQueue << std::endl;
//   std::cout << "Length shortest path: " << a << std::endl;
//   std::cout << "{";
//   for (int i = 0; i < a; i++) {
//      std::cout << pOutBuffer[i];
//      if (i != a - 1 ) {std::cout << ", ";}
//   }
//   std::cout << "}\n";
//   exit(0);
// }
