#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <atomic>
#include <vector>

template <typename T, size_t Capacity>
class LockFreeSPSC {
private:
    struct Node {
        T data;
    };

    Node buffer[Capacity];
    std::atomic<size_t> head{0};
    std::atomic<size_t> tail{0};

public:
    bool push(const T& item) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        size_t current_head = head.load(std::memory_order_acquire);

        if ((current_tail + 1) % Capacity == current_head) {
            return false; // Buffer full
        }

        buffer[current_tail].data = item;
        tail.store((current_tail + 1) % Capacity, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        size_t current_head = head.load(std::memory_order_relaxed);
        size_t current_tail = tail.load(std::memory_order_acquire);

        if (current_head == current_tail) {
            return false; // Buffer empty
        }

        item = buffer[current_head].data;
        head.store((current_head + 1) % Capacity, std::memory_order_release);
        return true;
    }
};

#endif // RING_BUFFER_H