#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <thread>
#include <atomic>

// Ultra-fast Lock-Free SPSC Ring Buffer for inter-thread tick streaming
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

struct MarketTick {
    uint64_t sequence_id;
    double bids[5];
    double asks[5];
};

class ApexRingBufferEngine {
public:
    void evaluateMarketPressure(const double* bids, const double* asks, int depth) {
        double bid_tensor_energy = 0.0;
        double ask_tensor_energy = 0.0;
        
        for(int i = 0; i < depth; ++i) {
            bid_tensor_energy += std::pow(bids[i], 3) * (depth - i); 
            ask_tensor_energy += std::pow(asks[i], 3) * (depth - i);
        }
        
        double total_energy = bid_tensor_energy + ask_tensor_energy;
        double buyer_probability = (total_energy > 0) ? (bid_tensor_energy / total_energy) * 100.0 : 50.0;
        double seller_probability = (total_energy > 0) ? (ask_tensor_energy / total_energy) * 100.0 : 50.0;
        
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "📈 Buyer Pressure: " << buyer_probability << "% | 📉 Seller Pressure: " << seller_probability << "%\n";
        
        if (buyer_probability > 58.0) {
            std::cout << "🚀 [SIGNAL] BULLISH EDGE DETECTED\n";
        } else if (seller_probability > 58.0) {
            std::cout << "⚠️ [SIGNAL] BEARISH EDGE DETECTED\n";
        } else {
            std::cout << "⚖️ [SIGNAL] NEUTRAL MARKET\n";
        }
    }
};

int main() {
    std::cout << "🏛️ PROJECT SINGULARITY v106.0: LOCK-FREE RING BUFFER ENGINE\n";
    std::cout << "=================================================================\n";
    
    LockFreeSPSCRingBuffer<MarketTick, 1024> ring_buffer;
    ApexRingBufferEngine engine;

    std::atomic<bool> producer_done{false};

    // Producer Thread: Simulates incoming high-speed market data feed
    std::thread producer([&]() {
        for (uint64_t i = 1; i <= 1000; ++i) {
            MarketTick tick;
            tick.sequence_id = i;
            tick.bids[0] = 150.2; tick.bids[1] = 150.1; tick.bids[2] = 150.0; tick.bids[3] = 149.8; tick.bids[4] = 149.5;
            tick.asks[0] = 150.5; tick.asks[1] = 150.6; tick.asks[2] = 150.8; tick.asks[3] = 151.0; tick.asks[4] = 151.2;
            
            while (!ring_buffer.push(tick)) {
                // Spin-wait if buffer is full (Zero-latency polling)
            }
        }
        producer_done.store(true, std::memory_order_release);
    });

    // Consumer Thread: Consumes ticks from ring buffer and evaluates signals
    std::thread consumer([&]() {
        MarketTick tick;
        uint64_t processed = 0;
        
        auto start = std::chrono::high_resolution_clock::now();

        while (processed < 1000) {
            if (ring_buffer.pop(tick)) {
                // Print sample signals for selected ticks to keep log readable
                if (processed == 0 || processed == 500 || processed == 999) {
                    std::cout << "\n--- Processing Ring Buffer Tick ID: " << tick.sequence_id << " ---\n";
                    engine.evaluateMarketPressure(tick.bids, tick.asks, 5);
                }
                processed++;
            } else if (producer_done.load(std::memory_order_acquire)) {
                if (!ring_buffer.pop(tick)) break;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::cout << "\n=================================================================\n";
        std::cout << "⚡ Total Ticks Processed via Ring Buffer: " << processed << "\n";
        std::cout << "⚡ Total Execution Time (1,000 Ticks): " << duration << " ns\n";
        std::cout << "⚡ Average Latency per Tick: " << (duration / (double)processed) << " ns\n";
    });

    producer.join();
    consumer.join();

    return 0;
}