#include "sandpile.h"
#include "bmp.h"

#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

struct Node {
  int y;
  int x;
  Node* next = nullptr;
  Node(int _y, int _x): y(_y), x(_x) {}
  ~Node() {
    delete next;
  }
};

struct Queue {
  Node* head = nullptr;
  Node* tail = nullptr;

  bool IsEmpty() const { return head == nullptr; }

  void Push(int y, int x) {
    Node* p = new Node(y, x);
    if (IsEmpty()) {
      head = p;
      tail = p;
      return;
    }
    tail->next = p;
    tail = p;
  }

  Node* Pop() {
    Node* temp = head;
    head = head->next;
    return temp;
  }

  ~Queue() {
    delete head;
  }
};


void Sandpile::FromTsv(const char* directory_tsv) {

  std::ifstream file(directory_tsv);
  if (!file.is_open()) {
    std::cout << "Unknown file" << '\n';
    std::exit(EXIT_FAILURE);
  }

  int x, y, grains;
  file >> x >> y >> grains;
  int max_x = x, min_x = x;
  int max_y = y, min_y = y;
  while (!file.eof()) {
    file >> x >> y >> grains;
    max_x = std::max(x, max_x);
    max_y = std::max(y, max_y);
    min_x = std::min(x, min_x);
    min_y = std::min(y, min_y);
  }
  file.clear();
  file.seekg(0, std::ios::beg);

  width = max_x - min_x + 1;
  height = max_y - min_y + 1;
  grid = new uint64_t*[height];

  for (uint64_t i = 0; i < height; ++i) {
    grid[i] = new uint64_t[width];
    for (uint64_t j = 0; j < width; ++j) {
      grid[i][j] = 0;
    }
    std::memset(grid[i], 0, width);
  }

  while (!file.eof()) {
    file >> x >> y >> grains;
    grid[y - min_y][x - min_x] = grains;
  }
}

void Sandpile::Print() const{
  for (uint64_t i = 0; i < height; ++i) {
    for (uint64_t j = 0; j < width; ++j) {
      std::cout << grid[i][j] << " ";
    } std::cout << '\n';
  }
}

void Sandpile::Expand()   {
  int is_expand_up = 0;
  int is_expand_down = 0;
  for (size_t x = 0; x < width; ++x) {
    if (grid[0][x] > kMaxPileInCell - 1) {
      is_expand_up = 1;
    }
    if (grid[height - 1][x] > kMaxPileInCell - 1) {
      is_expand_down = 1;
    }
  }
  int is_expand_left = 0;
  int is_expand_right = 0;
  for (size_t y = 0; y < height; ++y) {

    if (grid[y][0] > kMaxPileInCell - 1) {
      is_expand_left = 1;
    }
    if (grid[y][width - 1] > kMaxPileInCell - 1) {
      is_expand_right = 1;
    }
  }
  // Expand
  if ((is_expand_left + is_expand_right + is_expand_down + is_expand_up) != 0) {
    uint64_t** temp_grid = new uint64_t*[height];
    for (size_t y = 0; y < height; ++y) {
      temp_grid[y] = new uint64_t[width];
      for (size_t x = 0; x < width; ++x) {
        temp_grid[y][x] = grid[y][x];
      }
    }
    for (size_t y = 0; y < height; ++y) {
      delete[] grid[y];
    }
    delete[] grid;
    width += is_expand_left + is_expand_right;
    height += is_expand_up + is_expand_down;
    grid = new uint64_t*[height];
    for (size_t y = 0; y < height; ++y) {
      grid[y] = new uint64_t[width];
      for (size_t x = 0; x < width; ++x) {
        grid[y][x] = 0;
      }
    }

    for (size_t y = 0; y < height - is_expand_up - is_expand_down; ++y) {
      for (size_t x = 0; x < width - is_expand_left - is_expand_right; ++x) {
        grid[y + is_expand_left][x + is_expand_up] = temp_grid[y][x];
      }
    }
    for (size_t y = 0; y < height - is_expand_up - is_expand_down; ++y) {
      delete[] temp_grid[y];
    }
    delete[] temp_grid;
  }
}

void Sandpile::MakeIteration() {
  Expand();
  Queue q;
  for (uint64_t y = 1; y < height - 1; ++y) {
    for (uint64_t x = 1; x < width - 1; ++x) {
      if (grid[y][x] > kMaxPileInCell - 1) {
        grid[y][x] -= kMaxPileInCell;
        q.Push(y, x);
      }
    }
  }
  while (!q.IsEmpty()) {
    Node* temp = q.Pop();
    ++grid[temp->y][temp->x - 1];
    ++grid[temp->y][temp->x + 1];
    ++grid[temp->y - 1][temp->x];
    ++grid[temp->y + 1][temp->x];
  }
}

bool Sandpile::IsNeedIteration() const {
  for (uint64_t y = 0; y < height; ++y) {
    for (uint64_t x = 0; x < width; ++x) {
      if (grid[y][x] > kMaxPileInCell - 1) {
        return true;
      }
    }
  }
  return false;
}

void Sandpile::SaveToBmp(std::string& path) const {
  Bmp bmp;
  bmp.SaveToBmp(path, grid, height, width);
}