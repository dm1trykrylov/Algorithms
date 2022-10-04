#include <iostream>
#include <optional>
#include <string>
#include <vector>

struct Element {
  long long val;
  size_t query_id;
};

bool operator<(Element a, Element b) { return a.val < b.val; }

template <typename T>
class MinHeap {
  static constexpr size_t kCapacity = 8;
  static constexpr size_t kTopIdx = 1;
  const size_t kMaxQueriesCount = 1e6 + 2;
  std::vector<T> data_;
  std::vector<size_t> position_;
  size_t size_;
  int type_;  // type == 1 => minHeap, type == -1 => maxHeap

 public:
  MinHeap()
      : data_(kCapacity), position_(kMaxQueriesCount), size_(0), type_(1) {}

  MinHeap(int t)
      : data_(kCapacity), position_(kMaxQueriesCount), size_(0), type_(t) {}

  std::optional<T> Top() {
    if (size_ == 0) {
      return std::nullopt;
    }
    return data_[kTopIdx];
  }

  std::optional<T> GetMin() {
    if (size_ == 0) {
      return std::nullopt;
    }
    return data_[kTopIdx];
  }

  std::optional<T> ExtractMin() {
    if (size_ == 0) {
      return std::nullopt;
    }
    std::swap(position_[data_[kTopIdx].query_id],
              position_[data_[size_].query_id]);
    std::swap(data_[kTopIdx], data_[size_]);
    --size_;
    SiftDown(kTopIdx);
    return data_[size_ + 1];
  }

  void ExtractElement(size_t q_index) {
    if (size_ > 0) {
      size_t index = position_[q_index];
      std::swap(position_[q_index], position_[data_[size_].query_id]);
      std::swap(data_[index], data_[size_]);
      --size_;
      SiftUp(index);
      SiftDown(index);
    }
  }

  void Insert(T val) {
    if (size_ == data_.size() - 1) {
      data_.resize(2 * size_);
    }
    data_[++size_] = val;
    position_[val.query_id] = size_;
    SiftUp(size_);
  }

  size_t Size() { return size_; }

  bool Empty() { return size_ == 0; }

  void Clear() {
    data_.clear();
    data_.resize(kCapacity);
    size_ = 0;
  }

 private:
  void SiftUp(size_t index) {
    if (index > kTopIdx) {
      size_t parent = index / 2;
      if (data_[index].val * type_ < data_[parent].val * type_) {
        std::swap(position_[data_[index].query_id],
                  position_[data_[parent].query_id]);
        std::swap(data_[index], data_[parent]);
        SiftUp(parent);
      }
    }
  }

  void SiftDown(size_t index) {
    size_t i_min = index;
    size_t i_left_child = index * 2;
    size_t i_right_child = index * 2 + 1;
    if (i_left_child <= size_ &&
        data_[i_left_child].val * type_ < data_[i_min].val * type_) {
      i_min = i_left_child;
    }
    if (i_right_child <= size_ &&
        data_[i_right_child].val * type_ < data_[i_min].val * type_) {
      i_min = i_right_child;
    }
    if (index != i_min) {
      std::swap(position_[data_[i_min].query_id],
                position_[data_[index].query_id]);
      std::swap(data_[i_min], data_[index]);
      SiftDown(i_min);
    }
  }
};

void ExtractMin(MinHeap<Element>* min_heap, MinHeap<Element>* max_heap) {
  auto min_e = min_heap->ExtractMin();
  if (min_e.has_value()) {
    max_heap->ExtractElement((*min_e).query_id);
    std::cout << (*min_e).val << '\n';
  } else {
    std::cout << "error" << '\n';
  }
}

void GetMin(MinHeap<Element>* min_heap) {
  auto min_e = min_heap->GetMin();
  if (min_e.has_value()) {
    std::cout << (*min_e).val << '\n';
  } else {
    std::cout << "error" << '\n';
  }
}

template <typename T>
void ProcessCommand(std::string command, size_t idx, MinHeap<Element>* min_heap,
                    MinHeap<Element>* max_heap) {
  if (command == "insert") {
    T x;
    std::cin >> x;
    min_heap->Insert({x, idx});
    max_heap->Insert({x, idx});
    std::cout << "ok" << '\n';
  } else if (command == "get_min") {
    GetMin(min_heap);
  } else if (command == "extract_min") {
    ExtractMin(min_heap, max_heap);
  } else if (command == "get_max") {
    GetMin(max_heap);
  } else if (command == "extract_max") {
    ExtractMin(max_heap, min_heap);
  } else if (command == "size") {
    std::cout << min_heap->Size() << '\n';
  } else if (command == "clear") {
    min_heap->Clear();
    max_heap->Clear();
    std::cout << "ok" << '\n';
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t q;
  std::string command;
  auto min_heap = new MinHeap<Element>;
  auto max_heap = new MinHeap<Element>(-1);
  std::cin >> q;

  for (size_t i = 0; i < q; ++i) {
    std::cin >> command;
    ProcessCommand<long long>(command, i, min_heap, max_heap);
  }

  delete min_heap;
  delete max_heap;
  return 0;
}
