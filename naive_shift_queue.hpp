#ifndef NAIVE_SHIFT_QUEUE_HPP
#define NAIVE_SHIFT_QUEUE_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>

// 結果正確但效率較低的佇列。
// dequeue() 每次都將剩餘元素向前搬移一格。
class NaiveShiftQueue {
public:
    bool empty() const {
        return data_.empty();
    }

    std::size_t size() const {
        return data_.size();
    }

    int front() const {
        if (data_.empty()) {
            throw std::underflow_error("front() on empty queue");
        }
        return data_.front();
    }

    void enqueue(int value) {
        data_.push_back(value);
    }

    void dequeue() {
        if (data_.empty()) {
            throw std::underflow_error("dequeue() on empty queue");
        }
        for (std::size_t i = 1; i < data_.size(); ++i) {
            data_[i - 1] = data_[i];
        }
        data_.pop_back();
    }

private:
    std::vector<int> data_;
};

#endif
