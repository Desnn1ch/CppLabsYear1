#include <cstring>
#include "number.h"

bool IsNegative(const int2023_t& data) {
  return ((data.number[0] & 128) == 128);
}

int2023_t Reverse(const int2023_t& date) {
  int2023_t result;

  for (int i = int2023_t::kSize - 1; i >= 0; i--) {
    result.number[i] = ~date.number[i];
  }
  int i = int2023_t::kSize - 1;
  while (result.number[i] + 1 > int2023_t::kBase - 1) {
    result.number[i] = 0;
    i--;
  }
  result.number[i] += 1;
  return result;
}

int2023_t InsertNumber(uint64_t x, const int shift) {
  int2023_t result = from_int(0);

  int i = int2023_t::kSize - 1;
  while (x > 0 && i - shift >= 0) {
    result.number[i - shift] = x % int2023_t::kBase;
    x /= int2023_t::kBase;
    i--;
  }

  return result;
}

int2023_t from_int(int32_t x) {
  int2023_t result;

  for (int i = 0; i < int2023_t::kSize; i++) {
    result.number[i] = 0;
  }

  bool is_negative = false;
  if (x < 0) {
    is_negative = true;
    x = abs(x);
  }

  int i = int2023_t::kSize - 1;
  while (x > 0) {
    result.number[i] = x % int2023_t::kBase;
    x /= int2023_t::kBase;
    i--;
  }
  if (is_negative) {
    result = Reverse(result);
  }

  return result;
}

int2023_t from_string(const char* buff) {
  int2023_t result;
  result = from_int(0);
  int is_minus = 0;
  if (buff[0] == '-') {
    is_minus = 1;
  }
  size_t size = std::strlen(buff);

  int* date = new int[size - is_minus];
  for (int i = size - 1; i >= is_minus; i--) {
    date[size - 1 - i] = (buff[i] - '0');
  }

  size -= is_minus;

  int base = 10;
  int sign, current, count = 0;
  int j = (int2023_t::kSize - 1) * 8 + 7, add_base = 1;

  while (count < size) {
    sign = 0;
    for (int i = size - 1; i >= 0; i--) {
      current = date[i] + sign * base;
      date[i] = current / 2;
      sign = current % 2;
    }

    if (date[size - 1 - count] == 0) {
      count++;
    }
    result.number[j / 8] |= sign * add_base;
    add_base *= 2;
    j--;
    if (add_base >= 256) {
      add_base = 1;
    }
  }
  if (is_minus == 1) {
    result = Reverse(result);
  }

  delete [] date;
  return result;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result = from_int(0);
  int sm = 0;
  int overflow = 0;

  for (int i = int2023_t::kSize - 1; i >= 0; i--) {
    sm = lhs.number[i] + rhs.number[i] + overflow;
    result.number[i] = sm % int2023_t::kBase;
    overflow = sm / int2023_t::kBase;
  }

  return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t temp_rhs = Reverse(rhs);
  return lhs + temp_rhs;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result = from_int(0);
  int2023_t temp_lhs = lhs, temp_rhs = rhs;
  int2023_t temp;

  bool is_negative_lhs = IsNegative(lhs), is_negative_rhs = IsNegative(rhs);
  bool is_negative_ans = false;
  if (is_negative_lhs && is_negative_rhs) {
    temp_lhs = Reverse(lhs);
    temp_rhs = Reverse(rhs);
  } else if (is_negative_lhs) {
    temp_lhs = Reverse(lhs);
    is_negative_ans = true;
  } else if (is_negative_rhs) {
    temp_rhs = Reverse(rhs);
    is_negative_ans = true;
  }

  for (int i = int2023_t::kSize - 1, c = 0; i > 0; i--, c++) {
    for (int j = int2023_t::kSize - 1, k = 0; j > 0; j--, k++) {
      temp = InsertNumber(temp_lhs.number[i] * temp_rhs.number[j], c + k);
      result = result + temp;
    }
  }
  if (is_negative_ans) {
    result = Reverse(result);
  }
  return result;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t temp_lhs = lhs, temp_rhs = rhs;
  bool is_lhs_negative = IsNegative(lhs), is_rhs_negative = IsNegative(rhs);
  bool is_result_negative = false;


  if (is_lhs_negative && is_rhs_negative){
    temp_lhs = Reverse(lhs);
    temp_rhs = Reverse(rhs);
  } else if (is_lhs_negative) {
    temp_lhs = Reverse(temp_lhs);
    is_result_negative = true;
  } else if (is_rhs_negative) {
    temp_rhs = Reverse(temp_rhs);
    is_result_negative = true;
  }

  if ((temp_lhs == from_int(0)) || (temp_lhs == from_int(0)) || (temp_lhs < temp_rhs)) {
    return from_int(0);
  }

  int2023_t result = from_int(0);

  while (temp_lhs >= temp_rhs) {
    int2023_t divider = temp_rhs;
    int2023_t temp_counter = from_int(1);

    while (temp_lhs >= (divider + divider)) {
      divider = divider + divider;
      temp_counter = temp_counter + temp_counter;
    }

    temp_lhs = temp_lhs - divider;
    result = result + temp_counter;
  }

  if (is_result_negative) {
    result = Reverse(result);
  }
  return result;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
  for (int i = 0; i < int2023_t::kSize; i++) {
    if (lhs.number[i] != rhs.number[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
  return !(lhs == rhs);
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs) {

  bool is_negative_lhs = IsNegative(lhs);
  bool is_negative_rhs = IsNegative(rhs);

  if ((!is_negative_lhs) && is_negative_rhs) {
    return true;
  } else if (is_negative_lhs && (!is_negative_rhs)) {
    return false;
  }

  for (ssize_t i = 0; i < int2023_t::kSize; ++i) {
    if (lhs.number[i] > rhs.number[i]) {
      return true;
    } else if (lhs.number[i] < rhs.number[i]) {
      return false;
    }
  }
  return false;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
  return (!(lhs >= rhs));
}

bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {
  return ((lhs == rhs) || (lhs > rhs));
}

bool operator<=(const int2023_t& lhs, const int2023_t& rhs) {
  return (!(lhs > rhs));
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
  int2023_t temp = value;

  if (IsNegative(temp)) {
    stream << "-";
    temp = Reverse(value);
  }
  if (((temp.number[0]) & 127) != 0) {
    stream << (temp.number[0]) % int2023_t::kBase << " ";
  }
  int i = 1;
  while ((temp.number[i]) == 0 && i < int2023_t::kSize) {
    i++;
  }

  for (i; i < int2023_t::kSize; i++) {
    stream << ((temp.number[i]) % int2023_t::kBase) << ' ';
  }

  return stream;

}
