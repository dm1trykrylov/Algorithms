#include <algorithm>
#include <iostream>
#include <vector>

const uint64_t kByte = 255;
const size_t kMaxBytes = 8;

uint64_t GetByte(uint64_t number, size_t byte_idx) {
  size_t offset = byte_idx * 8;
  uint64_t tmp = kByte << offset;
  tmp = number & tmp;
  tmp >>= offset;
  return tmp;
}

void LSDsort(size_t n, std::vector<uint64_t>& array) {
  std::vector<uint64_t> bytes_count(kByte + 1);
  std::vector<uint64_t> prefix(kByte + 1);
  std::vector<uint64_t> result(array.size());

  for (size_t byte_idx = 0; byte_idx < kMaxBytes; ++byte_idx) {
    for (auto& number : array) {
      ++bytes_count[GetByte(number, byte_idx)];
    }
    prefix[0] = 0;
    for (size_t i = 1; i < prefix.size(); ++i) {
      prefix[i] = prefix[i - 1] + bytes_count[i - 1];
    }
    for (size_t i = 0; i < n; ++i) {
      result[(prefix[GetByte(array[i], byte_idx)]++)] = array[i];
    }
    std::fill(bytes_count.begin(), bytes_count.end(), 0);
    for (size_t i = 0; i < n; ++i) {
      array[i] = result[i];
    }
  }
  array = result;
}

void LSDsortMem(size_t n, std::vector<uint64_t>& array) { // Seems to be more memory eficient
  std::vector<uint64_t> bytes_count(kByte + 1);
  std::vector<uint64_t> result(array.size());
  uint64_t tmp, count, d;

  for (size_t byte_idx = 0; byte_idx < kMaxBytes; ++byte_idx) {
    for (auto& number : array) {
      ++bytes_count[GetByte(number, byte_idx)];
    }
    count = 0;
    for (size_t i = 0; i <= kByte; ++i) {
      tmp = bytes_count[i];
      bytes_count[i] = count;
      count += tmp;
    }
    for (size_t i = 0; i < n; ++i) {
      d = GetByte(array[i], byte_idx);
      result[bytes_count[d]] = array[i];
      ++bytes_count[d];
    }
    std::fill(bytes_count.begin(), bytes_count.end(), 0);
    for (size_t i = 0; i < n; ++i) {
      array[i] = result[i];
    }
  }
  array = result;
}

