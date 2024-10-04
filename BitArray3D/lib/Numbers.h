#pragma once
#include <cstdint>
#include <iostream>

static const uint32_t kCounter8Bits = 8;
static const uint32_t kCounter16Bits = 16;
static const uint32_t kMaskFirst16Bits = 0xFFFF;

class uint17_t {
 public:
  uint16_t* first_16_bits_;
  uint8_t* last_bit_;
  size_t index_;
  uint17_t(size_t index, uint16_t* first_16_bits, uint8_t* last_bit);
  void operator=(int value);
  void operator=(const uint17_t& other);
  operator int() const;
  void operator+=(const uint17_t& other);
  void operator-=(const uint17_t& other);
};

class WrapXYZ {
 public:
  WrapXYZ(size_t size_x, size_t size_y, size_t size_z, uint16_t* first_16_bits, uint8_t* last_bit,
          size_t index_x, size_t index_y);
  uint17_t operator[](size_t size_z) const;
 private:
  size_t size_x_;
  size_t size_y_;
  size_t size_z_;
  uint16_t* first_16_bits_;
  uint8_t* last_bit_;
  size_t index_x_;
  size_t index_y_;
};

class WrapXY {
 public:
  WrapXY(size_t size_x, size_t size_y, size_t size_z, uint16_t* first_16_bits, uint8_t* last_bit,
         size_t index_x);
  WrapXYZ operator[](size_t index_y) const;

 private:
  size_t size_x_;
  size_t size_y_;
  size_t size_z_;
  uint16_t* first_16_bits_;
  uint8_t* last_bit_;
  size_t index_x_;
};

class Numbers {
 public:
  Numbers(size_t size_x_, size_t size_y_, size_t size_z_);
  Numbers(const Numbers& other);
  static Numbers MakeArray(size_t size_x, size_t size_y, size_t size_z);
  Numbers& operator=(const Numbers& other);

  WrapXY operator[](size_t index_x) const;
  size_t GetX() const;
  size_t GetY() const;
  size_t GetZ() const;
  uint16_t* GetFirst16Bits() const;
  uint8_t* GetLastBit() const;

  ~Numbers();

 private:
  size_t size_x_;
  size_t size_y_;
  size_t size_z_;
  uint16_t* first_16_bits_;
  uint8_t* last_bit_;
};

std::ostream& operator<<(std::ostream& stream, const uint17_t& value);
std::ostream& operator<<(std::ostream& stream, Numbers& array);
std::istream& operator>>(std::istream& stream, const uint17_t& value);
std::istream& operator>>(std::istream& stream, Numbers& array);

Numbers operator+(const Numbers& arr1, const Numbers& arr2);
Numbers operator-(const Numbers& arr1, const Numbers& arr2);
Numbers operator*(const Numbers& arr, const int multi);
Numbers operator*(const int multi, const Numbers& arr);
