#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <atomic>
#include <cstddef>

template<typename T, size_t Capacity>
class LockFreeSPSCRingBuffer {
private:
    T buffer[Capacity];
    std::atomic<size_t> head{0};
    std::atomic<size_t> tail{0};

public:
    bool push(const T& item) {
        size_t current_head = head.load(std::memory_order_relaxed);
        size_t next_head = (current_head + 1) % Capacity;
        if (next_head == tail.load(std::memory_order_acquire)) {
            return false; // Buffer full
        }
        buffer[current_head] = item;
        head.store(next_head, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        size_t current_tail = tail.load(std::memory_order_relaxed);
        if (current_tail == head.load(std::memory_order_acquire)) {
            return false; // Buffer empty
        }
        item = buffer[current_tail];
        tail.store((current_tail + 1) % Capacity, std::memory_order_release);
        return true;
    }
};

#endif // RING_BUFFER_H