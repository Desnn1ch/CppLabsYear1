#pragma once

#include <iostream>
#include <cinttypes>

struct Arguments {
  const char* directory_tsv;
  const char* directory_bmp;
  uint32_t iteration = 0;
  uint32_t frequency = 0;
};

Arguments ParseArguments(int argc, char** argv);

