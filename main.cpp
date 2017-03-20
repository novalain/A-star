#include <iostream>
#include <queue>
#include <fstream>
#include <chrono>

int popsFromQueue = 0;

int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY,
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize);

struct Node {
  int x;
  int y;
  int id;
  int f;
  int g;
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

int calculate_heuristic(std::string type, int x, int y, const int& tx, const int& ty) {
  return std::max(x - tx, tx - x) + std::max(y - ty, ty - y);
}

// Brute force BFS
int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY,
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize) {

  std::priority_queue<Node, std::vector<Node>, Compare> queue;
  std::vector<int> visited(nMapWidth * nMapHeight);
  std::vector<int> predecessors(nMapWidth * nMapHeight, -1);

  Node start = {nStartX, nStartY, id(nStartX, nStartY, nMapWidth), 0};
  queue.push(start);

  while (!queue.empty()) {
    Node node = queue.top();
    queue.pop();

    popsFromQueue++;

    //std::cout << "Investigating node with ID " << node.id << std::endl;
    //std::cout << "This node predecessor " << predecessors[node.id] << std::endl;
    if (node.x == nTargetX && node.y == nTargetY) {
      std::cout << "NODE IS FOUND WITH ID "<< node.id << std::endl;
      if (node.g > nOutBufferSize) {
        return -1;
      }
      Backtrack(pOutBuffer, predecessors, node.id, id(nStartX, nStartY, nMapWidth), nOutBufferSize);
      return node.g;
    }

    if (visited[node.id]) {
      //std::cout << "Node with id " << node.id << " already visited, aborting.. " << std::endl;
      continue;
    }
    visited[node.id] = 1;
    //std::cout << "node " << node.id << "IS VISITED NOT VISITING AGAIN EVER" << std::endl;
    {
      // Right
      Node neighbor = {node.x + 1, node.y, id(node.x + 1, node.y, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.x + 1 <= nMapWidth && pMap[neighbor.id] != 0 && pMap[neighbor.id] != 'S') {
          //std::cout << "PUSHING NODE " << neighbor.id << std::endl;
          int distance = node.g + 1;
          neighbor.g = distance;
          neighbor.f = distance + calculate_heuristic("manhattan", node.x + 1, node.y, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
      }
    }
    {
      // Left
      Node neighbor = {node.x - 1, node.y, id(node.x - 1, node.y, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.x > 0 && pMap[neighbor.id] != 0 && pMap[neighbor.id] != 'S') {
         // std::cout << "PUSHING NODE " << neighbor.id << std::endl;
          int distance = node.g + 1;
          neighbor.g = distance;
          neighbor.f = distance + calculate_heuristic("manhattan", node.x - 1, node.y, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
      }
    }
    // Top
    {
      Node neighbor = {node.x, node.y + 1, id(node.x, node.y + 1, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.y + 1 <= nMapHeight && pMap[neighbor.id] != 0 && pMap[neighbor.id] != 'S') {
          //std::cout << "PUSHING NODE " << neighbor.id << std::endl;
          int distance = node.g + 1;
          neighbor.g = distance;
          neighbor.f = distance + calculate_heuristic("manhattan", node.x, node.y + 1, nTargetX, nTargetY);
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
      }
    }
    {
      // Y-dir
      Node neighbor = {node.x, node.y - 1, id(node.x, node.y - 1, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.y > 0 && pMap[neighbor.id] != 0 && pMap[neighbor.id] != 'S') {
        //std::cout << "PUSHING NODE " << neighbor.id << std::endl;
        int distance = node.g + 1;
        neighbor.g = distance;
        neighbor.f = distance + calculate_heuristic("manhattan", node.x, node.y - 1, nTargetX, nTargetY);
        predecessors[neighbor.id] = node.id;
        queue.push(neighbor);
      }
    }
  }
  std::cout << "NOPE, NO NODES FOUND" << std::endl;
  // Fail
  return -1;
}

void fetchData(unsigned char* pMap, std::string filename) {
  std::ifstream file(filename);
  std::string str;
  bool startFetch = false;
  int i = 0;
  while (std::getline(file, str)) {
    if (startFetch) {
      for(char& c : str) {
        pMap[i] = c;
        i++;
      }
    }
    if (str == "map") startFetch = true;
  }
}

int main() {

  // // Test case 1
  // const unsigned char pMap[] = {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};
  // const int startX = 0, startY = 0, endX = 1, endY = 2, mapWidth = 4, mapHeight = 3;
  // int pOutBufferSize = 12;
  // int pOutBuffer[12];

 // Test case 2
  // unsigned char pMap[] = {0, 0, 1, 0, 1, 1, 1, 0, 1};
  // int pOutBuffer[7];
  // int a = FindPath(2, 0, 0, 2, pMap, 3, 3, pOutBuffer, 7);
  // Expected output: -1

  // Custom
  unsigned const int mapWidth = 512;
  unsigned const int mapHeight = 512;
  unsigned char pMap[mapWidth * mapHeight];
  fetchData(pMap, "testcases/swampofsorrows.map");
  int pOutBuffer[mapWidth * mapHeight];
  int pOutBufferSize = mapWidth * mapHeight;
  int startX = 2, startY = 2, endX = 510, endY = 510;

  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  int a = FindPath(startX, startY, endX, endY, pMap, mapWidth, mapHeight, pOutBuffer, pOutBufferSize);
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

  std::cout << "Time taken: " << duration << "s" << std::endl;
  std::cout << "Pops from queue: " << popsFromQueue << std::endl;
  std::cout << "Length shortest path: " << a << std::endl;
  std::cout << "{";
  for (int i = 0; i < a; i++) {
     std::cout << pOutBuffer[i];
     if (i != a - 1 ) {std::cout << ", ";}
  }
  std::cout << "}\n";
  return 0;
}

// STARTX, STARTY, ENDX, ENDY, PMAP, PMAPWIDTH, PMAPHEIGHT, OUTBUF, OUTBUFSIZE

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



// Results
// Test case 1: Time taken: 674s, popsfromqueue: 6
// A* Test case 1: Time taken: 133s, popsfromqueue: 4

// Swamp of Sorrow UFC : 210290s, 321446 pops from queue
// Swamp of Sorrow A*: Time taken:   Pops from queue: 55841
