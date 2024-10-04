#include "../lib/sandpile.h"
#include "../lib/parsing.h"

#include <iostream>

int main(int argc, char* argv[]) {
  Arguments data = ParseArguments(argc, argv);
  Sandpile sandpile;
  sandpile.FromTsv(data.directory_tsv);
  uint32_t iteration = 0;
  while (data.iteration > iteration || (data.iteration == 0 && sandpile.IsNeedIteration())) {
    ++iteration;
    sandpile.MakeIteration();
    if (iteration % data.frequency == 0) {
      std::string save = (data.directory_bmp + std::to_string(iteration / data.frequency) + ".bmp");
      sandpile.SaveToBmp(save);
    }
  }

  return EXIT_SUCCESS;
}
