#include "circular_queue.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int failures = 0;

void check(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        ++failures;
    }
}

void test_constructor_and_empty_state() {
    bool threw = false;
    try {
        CircularQueue invalid(0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    check(threw, "initial_capacity = 0 should throw std::invalid_argument");

    CircularQueue q(4);
    check(q.empty(), "a new queue should be empty");
    check(q.size() == 0, "a new queue should have size 0");
    check(q.capacity() == 4, "capacity should equal the requested capacity");
}

void test_empty_exceptions() {
    CircularQueue q;

    bool front_threw = false;
    try {
        (void)q.front();
    } catch (const std::underflow_error&) {
        front_threw = true;
    }
    check(front_threw, "front() on an empty queue should throw std::underflow_error");

    bool dequeue_threw = false;
    try {
        q.dequeue();
    } catch (const std::underflow_error&) {
        dequeue_threw = true;
    }
    check(dequeue_threw, "dequeue() on an empty queue should throw std::underflow_error");
}

void test_basic_fifo_without_resize() {
    CircularQueue q(4);
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);

    check(!q.empty(), "queue should not be empty after enqueue()");
    check(q.size() == 3, "size should be 3");
    check(q.front() == 10, "first front should be 10");

    q.dequeue();
    check(q.size() == 2, "size should decrease after dequeue()");
    check(q.front() == 20, "second front should be 20");

    q.dequeue();
    check(q.front() == 30, "third front should be 30");
    q.dequeue();
    check(q.empty(), "queue should be empty after removing all elements");
}

void test_simple_growth_without_wrap() {
    CircularQueue q(2);
    for (int value = 1; value <= 5; ++value) {
        q.enqueue(value);
    }

    check(q.size() == 5, "size should be 5 after five enqueue() calls");
    check(q.capacity() == 8, "capacity should grow from 2 to 4 to 8");

    for (int expected = 1; expected <= 5; ++expected) {
        check(!q.empty(), "queue became empty too early after growth");
        if (!q.empty()) {
            check(q.front() == expected, "FIFO order should survive ordinary growth");
            q.dequeue();
        }
    }
    check(q.empty(), "queue should be empty after draining the grown queue");
}

}  // namespace

int main() {
    test_constructor_and_empty_state();
    test_empty_exceptions();
    test_basic_fifo_without_resize();
    test_simple_growth_without_wrap();

    if (failures == 0) {
        std::cout << "All public tests passed.\n";
        return 0;
    }
    std::cerr << failures << " public check(s) failed.\n";
    return 1;
}
