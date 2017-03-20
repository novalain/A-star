#include <iostream>
#include <queue>
#include <fstream>

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
  std::vector<int> visited(nMapWidth * nMapHeight);
  std::vector<int> predecessors(nMapWidth * nMapHeight, -1);

  Node start = {nStartX, nStartY, id(nStartX, nStartY, nMapWidth), 0};
  queue.push(start);

  while (!queue.empty()) {
    Node node = queue.top();
    queue.pop();

    //std::cout << "Investigating node with ID " << node.id << std::endl;
    //std::cout << "This node predecessor " << predecessors[node.id] << std::endl;
    if (node.x == nTargetX && node.y == nTargetY) {
      std::cout << "NODE IS FOUND WITH ID "<< node.id << std::endl;
      if (node.f > nOutBufferSize) {
        return -1;
      }
      Backtrack(pOutBuffer, predecessors, node.id, id(nStartX, nStartY, nMapWidth), nOutBufferSize);
      return node.f;
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
          neighbor.f = node.f + 1;
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
      }
    }
    {
      // Left
      Node neighbor = {node.x - 1, node.y, id(node.x - 1, node.y, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.x > 0 && pMap[neighbor.id] != 0 && pMap[neighbor.id] != 'S') {
         // std::cout << "PUSHING NODE " << neighbor.id << std::endl;
          neighbor.f = node.f + 1;
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
      }
    }
    // Top
    {
      Node neighbor = {node.x, node.y + 1, id(node.x, node.y + 1, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.y + 1 <= nMapHeight && pMap[neighbor.id] != 0 && pMap[neighbor.id] != 'S') {
          //std::cout << "PUSHING NODE " << neighbor.id << std::endl;
          neighbor.f = node.f + 1;
          predecessors[neighbor.id] = node.id;
          queue.push(neighbor);
      }
    }
    {
      // Y-dir
      Node neighbor = {node.x, node.y - 1, id(node.x, node.y - 1, nMapWidth), 0};
      if (!visited[neighbor.id] && neighbor.y > 0 && pMap[neighbor.id] != 0 && pMap[neighbor.id] != 'S') {
        //std::cout << "PUSHING NODE " << neighbor.id << std::endl;
        neighbor.f = node.f + 1;
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
  // int pOutBuffer[12];
  // int a = FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12);
  //Expected output: 3 and {1, 5, 9}

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
  int a = FindPath(2, 2, 501, 501, pMap, mapWidth, mapHeight, pOutBuffer, mapWidth * mapHeight);

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
