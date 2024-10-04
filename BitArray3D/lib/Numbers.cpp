#include "Numbers.h"

uint17_t::uint17_t(size_t index, uint16_t* first_16_bits, uint8_t* last_bit) : index_(index), first_16_bits_(first_16_bits), last_bit_(last_bit) {};

void uint17_t::operator=(int value) {
  first_16_bits_[index_] = value & kMaskFirst16Bits;
  last_bit_[index_ / kCounter8Bits] &= ~(1 << (index_ % kCounter8Bits));
  last_bit_[index_ / kCounter8Bits] |= ((value >> kCounter16Bits) & 0x01) << (index_% kCounter8Bits);
}

void uint17_t::operator=(const uint17_t& other) {
  first_16_bits_[index_] = other.first_16_bits_[other.index_];
  last_bit_[index_/ kCounter8Bits] &= ~(1 << (index_% kCounter8Bits));
  last_bit_[index_/ kCounter8Bits] |= ((other.last_bit_[other.index_/ kCounter8Bits] >> (other.index_% kCounter8Bits)) & 1) << (index_% kCounter8Bits);
}

uint17_t::operator int() const {
  int result = first_16_bits_[index_] | (((last_bit_[index_/ 8] & (1 << index_% 8)) >> (index_% 8)) << 16);
  return result;
}

void uint17_t::operator+=(const uint17_t& other) {
  *this = *this + other;
}

void uint17_t::operator-=(const uint17_t& other) {
  *this = *this - other;
}

Numbers::Numbers(size_t size_x, size_t size_y, size_t size_z) :
    size_x_(size_x),
    size_y_(size_y),
    size_z_(size_z),
    first_16_bits_(new uint16_t[size_x_ * size_y_ * size_z_]),
    last_bit_(new uint8_t[(size_x_ * size_y_ * size_z_) / kCounter8Bits + ((size_x_ * size_y_ * size_z_) % kCounter8Bits == 0 ? 0 : 1)])
{
  for (size_t i = 0; i < (size_x_ * size_y_ * size_z_); ++i) first_16_bits_[i] = 0;
  for (size_t i = 0; i < (size_x_ * size_y_ * size_z_) / kCounter8Bits + ((size_x_ * size_y_ * size_z_) % kCounter8Bits == 0 ? 0 : 1); ++i) last_bit_[i] = 0;
};

Numbers::Numbers(const Numbers& other) :
    size_x_(other.size_x_),
    size_y_(other.size_y_),
    size_z_(other.size_z_),
    first_16_bits_(new uint16_t[size_x_ * size_y_ * size_z_]),
    last_bit_(new uint8_t[(size_x_ * size_y_ * size_z_) / kCounter8Bits + ((size_x_ * size_y_ * size_z_) % kCounter8Bits == 0 ? 0 : 1)])
{
  for (size_t i = 0; i < size_x_ * size_y_ * size_z_; ++i) {
    first_16_bits_[i] = other.first_16_bits_[i];
  }
  for (size_t i = 0; i < (size_x_ * size_y_ * size_z_) / kCounter8Bits + ((size_x_ * size_y_ * size_z_) % kCounter8Bits == 0 ? 0 : 1); ++i) {
    last_bit_[i] = other.last_bit_[i];
  }
}

Numbers Numbers::MakeArray(size_t size_x_, size_t size_y_, size_t size_z_) {
  return Numbers(size_x_, size_y_, size_z_);
}

Numbers& Numbers::operator=(const Numbers& other) {
  if (this != &other) {
    delete[] first_16_bits_;
    delete[] last_bit_;

    size_x_ = other.size_x_;
    size_y_ = other.size_y_;
    size_z_ = other.size_z_;
    first_16_bits_ = new uint16_t[size_x_ * size_y_ * size_z_];
    last_bit_ = new uint8_t[(size_x_ * size_y_ * size_z_) / kCounter8Bits + ((size_x_ * size_y_ * size_z_) % kCounter8Bits == 0 ? 0 : 1)];
    for (size_t i = 0; i < size_x_ * size_y_ * size_z_; ++i) {
      first_16_bits_[i] = other.first_16_bits_[i];
    }
    for (size_t i = 0; i < (size_x_ * size_y_ * size_z_) / kCounter8Bits + ((size_x_ * size_y_ * size_z_) % kCounter8Bits == 0 ? 0 : 1); ++i) {
      last_bit_[i] = other.last_bit_[i];
    }
  }
  return *this;
}

WrapXYZ::WrapXYZ(size_t size_x, size_t size_y, size_t size_z, uint16_t* first_16_bits, uint8_t* last_bit,
                          size_t index_x, size_t index_y)
    : size_x_(size_x), size_y_(size_y), size_z_(size_z), first_16_bits_(first_16_bits), last_bit_(last_bit), index_x_(index_x), index_y_(index_y)   {}

uint17_t WrapXYZ::operator[](size_t index_z) const {

  if (index_x_ < 0 || index_y_ < 0 || index_z < 0 || index_x_ >= size_x_ || index_y_ >= size_y_ || index_z >= size_z_) {
    std::cout << "undefined behaviour\n";
    exit(EXIT_FAILURE);
  }
  size_t index_= ((index_x_ * size_y_ + index_y_) * size_z_) + index_z;
  return uint17_t(index_, first_16_bits_, last_bit_);
}

WrapXY::WrapXY(size_t size_x, size_t size_y, size_t size_z, uint16_t* first_16_bits, uint8_t* last_bit, size_t index_x)
    : size_x_(size_x), size_y_(size_y), size_z_(size_z), first_16_bits_(first_16_bits), last_bit_(last_bit), index_x_(index_x) {}

WrapXYZ WrapXY::operator[](size_t index_y) const{
  return WrapXYZ(size_x_, size_y_, size_z_, first_16_bits_, last_bit_, index_x_, index_y);
}

WrapXY Numbers::operator[](size_t index_x) const {
  return WrapXY(size_x_, size_y_, size_z_, first_16_bits_, last_bit_, index_x);
}

size_t Numbers::GetX() const {
  return size_x_;
}

size_t Numbers::GetY() const {
  return size_y_;
}

size_t Numbers::GetZ() const {
  return size_z_;
}

uint16_t* Numbers::GetFirst16Bits() const {
  return first_16_bits_;
}

uint8_t* Numbers::GetLastBit() const {
  return last_bit_;
}

Numbers::~Numbers() {
  delete[] first_16_bits_;
  delete[] last_bit_;
}

std::ostream& operator<<(std::ostream& stream, const uint17_t& value)  {
  int temp = value.first_16_bits_[value.index_];
  temp |= ((value.last_bit_[value.index_/ kCounter8Bits] >> (value.index_% kCounter8Bits)) & 1) << kCounter16Bits;
  stream << temp;
  return stream;
};

std::ostream& operator<<(std::ostream& stream, Numbers& array) {
  for (size_t i = 0; i < array.GetX(); ++i) {
    for (size_t j = 0; j < array.GetY(); ++j) {
      for (size_t m = 0; m < array.GetZ(); ++m) {
        stream << array[i][j][m] << " ";
      }
      stream << '\n';
    }
    stream << '\n';
  }
  stream << '\n';
  return stream;
}

std::istream& operator>>(std::istream& stream, const uint17_t& value) {
  int temp;
  stream >> temp;
  value.first_16_bits_[value.index_] = temp & kMaskFirst16Bits;
  value.last_bit_[value.index_/ kCounter8Bits] &= ~(1 << (value.index_% kCounter8Bits));
  value.last_bit_[value.index_/ kCounter8Bits] |= ((temp >> kCounter16Bits) & 0x01) << (value.index_% kCounter8Bits);
  return stream;
}

std::istream& operator>>(std::istream& stream, Numbers& array) {
  for (size_t i = 0; i < array.GetX(); ++i) {
    for (size_t j = 0; j < array.GetY(); ++j) {
      for (size_t m = 0; m < array.GetZ(); ++m) {
        stream >> array[i][j][m];
      }
    }
  }
  return stream;
}

Numbers operator+(const Numbers& lhs, const Numbers& rhs) {
  Numbers result(lhs.GetX(), lhs.GetY(), lhs.GetZ());
  for (size_t i = 0; i < lhs.GetX(); ++i) {
    for (size_t j = 0; j < lhs.GetY(); ++j) {
      for (size_t m = 0; m < lhs.GetZ(); ++m) {
        result[i][j][m] = lhs[i][j][m] + rhs[i][j][m];
      }
    }
  }
  return result;
}

Numbers operator-(const Numbers& lhs, const Numbers& rhs) {
  Numbers result(lhs.GetX(), lhs.GetY(), lhs.GetZ());
  for (size_t i = 0; i < lhs.GetX(); ++i) {
    for (size_t j = 0; j < lhs.GetY(); ++j) {
      for (size_t m = 0; m < lhs.GetZ(); ++m) {
        result[i][j][m] = lhs[i][j][m] - rhs[i][j][m];
      }
    }
  }
  return result;
}

Numbers operator*(const Numbers& arr, const int multi) {
  Numbers result(arr.GetX(), arr.GetY(), arr.GetZ());
  for (size_t i = 0; i < arr.GetX(); ++i) {
    for (size_t j = 0; j < arr.GetY(); ++j) {
      for (size_t m = 0; m < arr.GetZ(); ++m) {
        result[i][j][m] = arr[i][j][m] * multi;
      }
    }
  }
  return result;
}

Numbers operator*(const int multi, const Numbers& arr) {
  return arr * multi;
}
