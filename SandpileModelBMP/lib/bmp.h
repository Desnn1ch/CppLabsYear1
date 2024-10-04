#pragma once

#include <cinttypes>
#include <string>
#include <fstream>

class Bmp {
 public:
  void SaveToBmp(std::string& path, uint64_t** grid, int h, int w);
 private:
  static const int kFileHeaderSize = 14;
  static const int kInfoSize = 40;
  static const int kPaletteSize = 16;
  static const int kCountColors = 5;

  uint8_t file_header[kFileHeaderSize];
  uint8_t file_info[kInfoSize] = {};
  uint32_t palette[kPaletteSize] = {
      //  RRGGBB
      0x00FFFFFF,
      0x0000FF00,
      0x00FFFF00,
      0x00FF00FF,
      0x00000000
  };
};
