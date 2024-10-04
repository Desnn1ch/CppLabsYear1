#include <iostream>
#include "../lib/Numbers.h"

int main() {
  Numbers array = Numbers::MakeArray(5, 5, 5);
  int temp = 0;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      for (int m = 0; m < 5; ++m) {
        array[i][j][m] = temp++;
      }
    }
  }
  bool res = true;
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      for (int m = 0; m < 5; ++m) {
        std::cout << array[i][j][m] << " ";
      }
    }
  }
  std::cout << '\n';
  Numbers new_arr = (array - array);
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      for (int m = 0; m < 5; ++m) {
          std::cout << array[i][j][m] << " ";
      }
    }
  }
  std::cout << res << '\n';

  return 0;
}