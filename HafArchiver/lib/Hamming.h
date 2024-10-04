#pragma once

#include "string"
#include "iostream"
#include "cstdint"
#include "vector"

const int kByte = 8;

size_t CountControlBits(size_t len);

std::vector<char> EncodeHam(const std::vector<char>& array);

std::vector<char> DecodeHam(const std::vector<char>& array);
