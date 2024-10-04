#include "Hamming.h"

size_t CountControlBits(size_t len) {
  size_t counter = 1;
  while ((1 << counter) < len + counter + 1) {
    ++counter;
  }
  return counter;
}

std::vector<char> EncodeHam(const std::vector<char>& array) {
  std::vector<char> temp;
  for (size_t _ = 0; _ < array.size(); ++_) {
    temp.push_back(0);
  }
  size_t count_control = CountControlBits(array.size() * kByte);

  size_t current = 0;
  for (size_t i = 0; i < (array.size() * kByte) + count_control; ++i) {
    if (i / kByte >= temp.size()) {
      temp.push_back(0);
    }
    if (!((i + 1) & i)) {
      temp[i / kByte] &= ~(1 << (kByte - 1 - i % kByte));
    } else {
      temp[i / kByte] |= ((array[current / kByte] >> (kByte - 1 - current % kByte)) & 1) << (kByte - 1 - i % kByte);
      ++current;
    }
  }

  for (size_t i = 0; i < count_control; ++i) {
    size_t sum = 0;
    for (size_t j = 1 << i; j < temp.size() * kByte; ++j) {
      if ((j + 1) & (1 << i)) {
        sum ^= (temp[j / kByte] >> (kByte - 1 - j % kByte)) & 1;
      }
    }
    temp[((1 << i) - 1) / kByte] |= (sum << (kByte - 1 - ((1 << i) - 1) % kByte));
  }

  return temp;
}

std::vector<char> DecodeHam(const std::vector<char>& array) {
  std::vector<uint8_t> temp;
  size_t bit_to_invert = 0;
  uint8_t c_bit = CountControlBits(array.size() * kByte);

  for (char el : array) {
    temp.push_back(el);
  }

  for (size_t i = 0; i < c_bit; ++i) {
    size_t sum = 0;
    for (size_t j = 1 << i; j < temp.size() * kByte; ++j) {
      if ((j + 1) & (1 << i)) {
        sum ^= (temp[j / kByte] >> (kByte - 1 - j % kByte)) & 1;
      }
    }
    if (sum != ((temp[((1 << i) - 1) / kByte] >> (kByte - 1 - ((1 << i) - 1) % kByte)) & 1)) {
      bit_to_invert += 1 << i;
    }
  }
  if (bit_to_invert) {
    --bit_to_invert;
    temp[bit_to_invert / kByte] ^= (1 << (kByte - 1 - bit_to_invert % kByte));
  }

  std::vector<char> res((array.size() * kByte - kByte) / kByte, 0);
  size_t current = 0;
  for (size_t i = 0; i < (array.size() * kByte) - (kByte - c_bit); ++i) {
    if (((i + 1) & i)) {
      res[current / kByte] |= ((temp[i / kByte] >> (kByte - 1 - i % kByte)) & 1) << (kByte - 1 - current % kByte);
      ++current;
    }
  }

  return res;
}
