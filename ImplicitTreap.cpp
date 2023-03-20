#include <cstdlib>
#include <iostream>
#include <random>
#include <utility>

namespace constants {
const int64_t k_max_val = 1e9 + 1;
}

template <typename T>
class TreapArray {
  struct Node {
    Node(int64_t priority, const T& value)
        : size(1),
          min(value),
          priority(priority),
          value(value),
          left(nullptr),
          right(nullptr) {}

    int64_t size;
    T min;
    int64_t priority;
    T value = 0;
    T add = 0;
    Node* left;
    Node* right;
  };

 public:
  TreapArray() : root_(nullptr) {}

  TreapArray(std::vector<T>& array) : root_(nullptr) {
    for (size_t i = 0; i < array.size(); ++i) {
      Insert(i, array[i]);
    }
  }

  ~TreapArray() { Clear(root_); }

  int64_t Size() { return Size(root_); }

  bool Empty() { return Size(root_) == 0; }

  void Erase(int64_t pos) {
    auto [left, right_with_pos] = Split(root_, pos);
    auto [pos_tree, right] = Split(right_with_pos, 1);
    delete pos_tree;
    root_ = Merge(left, right);
  }

  void Insert(int64_t pos, const T& value) {
    int64_t priority = distribution_(gen_);
    Node* node = new Node(priority, value);
    auto [first, second] = Split(root_, pos);
    root_ = Merge(Merge(first, node), second);
  }

  T GetMin(size_t left, size_t right) {
    auto [first, second_with_value] = Split(root_, left);
    auto [first_with_value, second] =
        Split(second_with_value, right + 1 - left);
    // Push(first_with_value);
    auto ans = first_with_value->min;
    root_ = Merge(first, Merge(first_with_value, second));
    return ans;
  }

  void Add(size_t left, size_t right, T increment) {
    auto [first, second_with_value] = Split(root_, left);
    auto [first_with_value, second] =
        Split(second_with_value, right + 1 - left);
    first_with_value->add += increment;
    root_ = Merge(first, Merge(first_with_value, second));
  }

  T at(int64_t pos) {
    auto [parent, pos_node] = Find(nullptr, root_, pos);
    return pos_node->value;
  }

  T& operator[](int64_t pos) {
    auto [parent, pos_node] = Find(nullptr, root_, pos);
    return pos_node->value;
  }

  void Print() {
    for (int64_t i = 0; i < Size(root_); ++i) {
      std::cout << at(i) << ' ';
    }
  }

 private:
  std::pair<Node*, Node*> Find(Node* parent, Node* node, int64_t pos) {
    Push(node);
    if (node == nullptr) {
      return {parent, node};
    }
    int64_t left_size = Size(node->left);
    if (pos == left_size) {
      return {parent, node};
    }
    if (pos < left_size) {
      return Find(node, node->left, pos);
    }
    return Find(node, node->right, pos - left_size - 1);
  }

  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->left);
    Clear(node->right);
    delete node;
  }

  Node* Merge(Node* first, Node* second) {
    Push(first);
    Push(second);
    if (first == nullptr) {
      return second;
    }
    if (second == nullptr) {
      return first;
    }
    if (first->priority > second->priority) {
      first->right = Merge(first->right, second);
      Update(first);
      return first;
    }
    second->left = Merge(first, second->left);
    Update(second);
    return second;
  }

  std::pair<Node*, Node*> Split(Node* node, int64_t pos) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    Push(node);
    int64_t left_size = Size(node->left);
    if (pos <= left_size) {
      auto [left, right] = Split(node->left, pos);
      node->left = right;
      Update(node);
      return {left, node};
    }
    auto [left, right] = Split(node->right, pos - left_size - 1);
    node->right = left;
    Update(node);
    return {node, right};
  }

  void Update(Node* node) {
    if (node == nullptr) {
      return;
    }
    Push(node);
    node->size = 1 + Size(node->left) + Size(node->right);
    node->min = std::min(node->value, Min(node->left));
    node->min = std::min(node->min, Min(node->right));
  }

  void Push(Node* node) {
    if (node == nullptr) {
      return;
    }
    if (node->left != nullptr) {
      node->left->add += node->add;
    }
    if (node->right != nullptr) {
      node->right->add += node->add;
    }
    node->min += node->add;
    node->value += node->add;  //* node->size;
    node->add = 0;
  }

  T Min(Node* node) {
    if (node == nullptr) {
      return constants::k_max_val;
    }
    Push(node);
    return node->min;
  }

  int64_t Size(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->size;
  }

  Node* root_;
  std::mt19937 gen_;
  std::uniform_int_distribution<int64_t> distribution_;
};

void FastIO() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
}

template <typename T>
void ProcessRequests(size_t count, TreapArray<T>& array) {
  char query;
  size_t vertex;
  size_t id = 1;
  for (size_t i = 0; i < count; ++i) {
    std::cin >> query;
    if (query == '+') {
      // array.Insert()
    }
  }
} 

int main() {
  FastIO();
  std::vector<int32_t> data = {1, 3, 5, 9, 2, 6, 4};
  auto lca_vertices = new TreapArray<int32_t>(data);

  lca_vertices->Print();

  delete lca_vertices;
  return 0;
}