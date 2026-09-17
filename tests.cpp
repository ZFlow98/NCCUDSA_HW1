#include "circular_queue.hpp"

#include <iostream>
#include <queue>
#include <random>
// #include <stdexcept>
#include <string>

namespace {

int failures = 0;
constexpr unsigned int kSeed =
    1795265022u; // TODO：改成你選定並記錄的固定亂數種子。

void check(bool condition, const std::string &message) {
  if (!condition) {
    std::cerr << "[FAIL] " << message << '\n';
    ++failures;
  }
}

void test_empty_and_one_element() {
  // TODO：完成「空佇列與單一元素測試」。
  // check(false, "TODO: test_empty_and_one_element");
  CircularQueue q(4);
  check(q.empty(), "Queue should be empty initially");
  check(q.size() == 0, "Size should be 0 initially");

  q.enqueue(61);
  check(!q.empty(), "Queue should not be empty after enqueue");
  check(q.size() == 1, "Size should be 1 after one enqueue");
  check(q.front() == 61, "Front element should be 61");

  q.dequeue();
  check(q.empty(), "Queue should be empty after dequeue");
  check(q.size() == 0, "Size should be 0");

  return;
}

void test_wrap_then_grow() {
  // TODO：完成「環狀回繞後擴充測試」。
  // 使用與答案紙最短反例相同的操作序列。
  // check(false, "TODO: test_wrap_then_grow");
  CircularQueue q(2);
  q.enqueue(9);
  q.dequeue();
  q.enqueue(5);
  q.enqueue(4);
  q.enqueue(6); // 觸發grow
  check(q.capacity() == 4, "Capacity should be double to 4");
  check(q.size() == 3, "Size should be 3");

  int verify[] = {5, 4, 6};
  for (int element : verify) {
    check(q.front() == element, "value mismatch");
    q.dequeue();
  }

  check(q.empty(), "Queue should be empty");
}

void test_randomized_differential() {
  // TODO：
  // 1. 使用上方的 kSeed 初始化亂數產生器。
  // 2. 產生至少 2,000 次合法的 enqueue()/dequeue() 操作。
  // 3. 將相同操作套用到 CircularQueue 與 std::queue<int>。
  // 4. 每一步比較 size()、empty()，非空時再比較 front()。
  // check(false, "TODO: test_randomized_differential");
  CircularQueue cq(2);
  std::queue<int> sq;

  std::mt19937 gen(kSeed);
  std::uniform_int_distribution<int> op_dist(0, 2); // 0,1: enqueue, 2: dequeue
  std::uniform_int_distribution<int> val_dist(1, 10000);

  for (int i = 0; i < 5000; ++i) {
    int op = op_dist(gen);
    if (op == 2 && !sq.empty()) { // 1/3 機率dequeue
      cq.dequeue();
      sq.pop();
    } else {
      int val = val_dist(gen);
      cq.enqueue(val);
      sq.push(val);
    }

    // 確認兩者是否等價
    check(cq.empty() == sq.empty(),
          "empty() status mismatched with std::queue");
    check(cq.size() == sq.size(), "size() mismatched with std::queue");
    if (!sq.empty()) {
      check(cq.front() == sq.front(),
            "front() value mismatched with std::queue");
    }
  }
}
} // namespace

int main() {
  test_empty_and_one_element();
  test_wrap_then_grow();
  test_randomized_differential();

  if (failures == 0) {
    std::cout << "All student tests passed.\n";
    return 0;
  }
  std::cerr << failures << " student check(s) failed.\n";
  return 1;
}
