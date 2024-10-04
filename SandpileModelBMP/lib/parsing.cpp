#include "parsing.h"

Arguments ParseArguments(int argc, char* argv[]) {
  Arguments result;

  for (size_t i = 1; i < argc; ++i) {
    if (i + 1 > argc) {
      std::cerr << "Need more arguments" << '\n';
      std::exit(EXIT_FAILURE);
    }
    std::string arg = argv[i];
    if (arg == "-i" || arg == "--input") {
      result.directory_tsv = argv[++i];
    } else if (arg == "-o" || arg == "--output") {
      result.directory_bmp = argv[++i];
    } else if (arg == "-m" || arg == "--max-iter") {
      result.iteration = std::stoi(argv[++i]);
    } else if (arg == "-f" || arg == "--freq") {
      result.frequency = std::stoi(argv[++i]);
    } else {
      std::cerr << "Unknown input" << '\n';
      std::exit(EXIT_FAILURE);
    }
  }

  return result;
}
