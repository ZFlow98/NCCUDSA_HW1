#include "circular_queue.hpp"

// #include <optional>
#include <cstddef>
#include <stdexcept>

CircularQueue::CircularQueue(std::size_t initial_capacity)
    : data_(nullptr), capacity_(initial_capacity), size_(0), front_(0),
      rear_(0) {
  if (initial_capacity == 0) {
    throw std::invalid_argument("Initial capacity must be greater than 0");
  }
  data_ = new int[initial_capacity];
}

CircularQueue::~CircularQueue() {
  delete[] data_;
  data_ = nullptr;
}

bool CircularQueue::empty() const { return size_ == 0; }

std::size_t CircularQueue::size() const { return size_; }

std::size_t CircularQueue::capacity() const { return capacity_; }

int CircularQueue::front() const {
  if (empty()) {
    throw std::underflow_error("Queue is empty");
  }
  return data_[front_];
}

void CircularQueue::grow() {
  // TODO 7：
  // 1. 將容量擴充為原本的兩倍。
  // 2. 依邏輯順序複製元素。
  // 3. 釋放舊陣列並更新 front_、rear_ 與 capacity_。
  int *new_data_ = new int[capacity_ * 2];
  size_t temp;
  for (temp = 0; temp < size_; ++temp) {
    new_data_[temp] = data_[(front_ + temp) % capacity_];
  }
  front_ = 0;
  capacity_ *= 2;
  rear_ = size_;
  delete[] data_;
  data_ = new_data_;

  return;
}

void CircularQueue::enqueue(int value) {
  // TODO 8：必要時先擴充容量，再將 value 加入隊尾。
  if (size_ == capacity_) {
    grow();
  }
  data_[rear_++] = value;
  size_++;
  rear_ %= capacity_;
  return;
}

void CircularQueue::dequeue() {
  if (size_ == 0) {
    throw std::underflow_error("Queue is empty");
  }
  front_ = (front_ + 1) % capacity_;
  --size_;

  return;
}
