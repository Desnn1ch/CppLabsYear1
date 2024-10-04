#include "bmp.h"

#include <string>
#include <fstream>
#include <cstring>

uint8_t GetColor(uint64_t count) {
  if (count > 4) {
    return static_cast<uint8_t>(4);
  }
  return static_cast<uint8_t>(count);
}

void Bmp::SaveToBmp(std::string& path, uint64_t** grid, int height, int width) {

  std::ofstream file(path, std::ios::out | std::ios::binary);

  int row = width / 2 + width % 2;
  int padding = (4 - (row % 4)) % 4;

  const int file_header_size = sizeof(file_header);
  const int file_info_size = sizeof(file_info);
  const int file_palette_size = sizeof(palette);

  int total = (row + padding) * height;

  std::memset(file_header, 0, kFileHeaderSize);
  //type               1  - 2
  file_header[0] = 'B';
  file_header[1] = 'M';
  //header_size        2  - 5
  file_header[2] = file_header_size;
  file_header[3] = file_header_size >> 8;
  file_header[4] = file_header_size >> 16;
  file_header[5] = file_header_size >> 24;
  //reserved           6  - 9
  //offset             10 - 13
  file_header[10] = file_header_size + file_info_size + file_palette_size;

  std::memset(file_info, 0, kInfoSize);
  //info size          0  - 3
  file_info[0] = file_info_size;
  //width              4  - 7
  file_info[4] = width;
  file_info[5] = width >> 8;
  file_info[6] = width >> 16;
  file_info[7] = width >> 24;
  //height             8  - 11
  file_info[8] = height;
  file_info[9] = height >> 8;
  file_info[10] = height >> 16;
  file_info[11] = height >> 24;
  //planes             12 - 13
  file_info[12] = 1;
  //bit per pixel      14 - 15
  file_info[14] = 4;
  //compression        16 - 19
  //image size         20 - 23
  //x pixels per meter 24 - 27
  //y pixels per meter 28 - 31
  //total colors       32 - 35
  file_info[32] = 16;
  //important colors   36 - 39

  file.write(reinterpret_cast<char*>(file_header), file_header_size);
  file.write(reinterpret_cast<char*>(file_info), file_info_size);
  file.write(reinterpret_cast<char*>(palette), file_palette_size);

  uint8_t* pixels = new uint8_t[total];
  size_t index = 0;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x += 2) {
      uint64_t first_pixel = grid[y][x];
      uint64_t second_pixel = (x + 1 < width) ? grid[y][x + 1] : 0;

      uint8_t color1 = GetColor(first_pixel);
      uint8_t color2 = GetColor(second_pixel);

      pixels[index++] = static_cast<uint8_t>((color1 << 4) | color2);
    }
    index += padding;
  }
  file.write(reinterpret_cast<char*>(pixels), total);
  delete [] pixels;
}

