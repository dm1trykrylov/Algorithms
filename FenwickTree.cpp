#include <algorithm>
#include <iostream>
#include <vector>

size_t GetLastBit(size_t number);

template <typename T>
class FenwickTree {
 public:
  FenwickTree(size_t size = kCapacity) {
    values_ = new T[kCapacity];
    size_ = size;
  }

  T GetSum(size_t right) {
    size_t sum = 0;
    for (; right > 0; right -= GetLastBit(right)) {
      sum += values_[right];
    }
    return sum;
  }

  T GetSum(size_t left, size_t right) {
    if (right < left) {
      return 0;
    }
    if (left == 0) {
      return GetSum(right);
    }
    return GetSum(right) - GetSum(left - 1);
  }

  void Add(size_t index, size_t incr = 1) {
    for (; index <= size_ + 1; index += GetLastBit(index)) {
      values_[index] += incr;
    }
  }

  ~FenwickTree() { delete[] values_; }

 private:
  T* values_;
  size_t size_;
  static constexpr size_t kCapacity = 1e7;
};

size_t GetLastBit(size_t number) {
  long long tmp = (long long)number;
  tmp = tmp & (-tmp);
  return (size_t)tmp;
}

template <typename T>
void CompressValues(std::vector<T>& values) {
  std::vector<T> tmp = values;
  std::sort(tmp.begin(), tmp.end());

  for (auto& v : values) {
    v = std::lower_bound(tmp.begin(), tmp.end(), v) - tmp.begin() + 1;
  }
}

template <typename T>
void ReadValues(size_t n, std::vector<T>& values) {
  values.resize(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> values[i];
  }
  CompressValues<T>(values);
}


