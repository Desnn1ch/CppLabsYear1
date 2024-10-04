#pragma once

#include "bmp.h"

#include <cinttypes>
#include <string>
#include <fstream>

class Sandpile {
 public:
  void FromTsv(const char* directory_tsv);

  void Print() const;

  void Expand();

  void MakeIteration();

  bool IsNeedIteration() const;

  void SaveToBmp(std::string& path) const;

 private:
  uint64_t** grid;
  size_t width;
  size_t height;
  int kMaxPileInCell = 4;
};
