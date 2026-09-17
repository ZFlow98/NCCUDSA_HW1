#include "circular_queue.hpp"
#include "naive_shift_queue.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

struct Measurement {
    double milliseconds;
    std::int64_t checksum;
};

template <typename Queue>
Measurement measure(std::size_t n) {
    Queue q;

    // 建立長度為 n 的初始佇列；這一階段不計時。
    for (std::size_t i = 0; i < n; ++i) {
        q.enqueue(static_cast<int>(i));
    }

    std::int64_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();

    for (std::size_t i = 0; i < n; ++i) {
        checksum += q.front();
        q.dequeue();
        q.enqueue(static_cast<int>(n + i));
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double, std::milli> elapsed = stop - start;
    return {elapsed.count(), checksum};
}

template <typename Queue>
void run_three_times(const std::string& name, std::size_t n) {
    for (int run = 1; run <= 3; ++run) {
        const Measurement result = measure<Queue>(n);
        std::cout << name << ',' << n << ',' << run << ','
                  << std::fixed << std::setprecision(6)
                  << result.milliseconds << ',' << result.checksum << '\n';
    }
}

}  // namespace

int main() {
    const std::size_t sizes[] = {1u << 9, 1u << 12, 1u << 15};

    std::cout << "implementation,n,run,milliseconds,checksum\n";
    for (const std::size_t n : sizes) {
        run_three_times<CircularQueue>("CircularQueue", n);
        run_three_times<NaiveShiftQueue>("NaiveShiftQueue", n);
    }
    return 0;
}
