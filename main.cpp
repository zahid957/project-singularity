#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include "ring_buffer.h"
#include "engine.h"
#include "network_transport.h"

// Ring buffer capacity (Power of 2 for fast bitwise wrapping)
const size_t RING_BUFFER_SIZE = 1024;
LockFreeSPSC<MarketTick, RING_BUFFER_SIZE> market_ring_buffer;

// Producer Thread: Simulates Exchange UDP Multicast Feed (Raw Network Ingestion)
void exchangeProducerThread() {
    uint64_t seq = 1;
    char network_raw_buffer[256]; // Simulated socket network buffer

    while (seq <= 1000) {
        double base_price = 150.0 + (std::sin(seq * 0.1) * 2.0);
        double sample_bids[5] = {base_price - 0.05, base_price - 0.10, base_price - 0.15, base_price - 0.20, base_price - 0.25};
        double sample_asks[5] = {base_price + 0.05, base_price + 0.10, base_price + 0.15, base_price + 0.20, base_price + 0.25};

        size_t packet_size = UDPNetworkTransportSimulator::createSimulatedExchangePacket(seq, sample_bids, sample_asks, network_raw_buffer);

        MarketTick incoming_tick;
        if (UDPNetworkTransportSimulator::parseRawPacket(network_raw_buffer, packet_size, incoming_tick)) {
            while (!market_ring_buffer.push(incoming_tick)) {
                std::this_thread::yield();
            }
        }

        seq++;
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}

// Consumer Thread: Ultra-Low Latency Execution Engine
void hftConsumerThread(std::vector<double>& latency_log) {
    ApexInstitutionalEngine execution_engine;
    MarketTick current_tick;
    uint64_t processed_count = 0;

    while (processed_count < 1000) {
        auto start_time = std::chrono::high_resolution_clock::now();

        if (market_ring_buffer.pop(current_tick)) {
            execution_engine.evaluateAndExecute(current_tick.bids, current_tick.asks, 5);

            auto end_time = std::chrono::high_resolution_clock::now();
            double duration_ns = std::chrono::duration<double, std::nano>(end_time - start_time).count();
            latency_log.push_back(duration_ns);

            processed_count++;
        } else {
            std::this_thread::yield();
        }
    }
}

int main() {
    std::cout << "=================================================================\n";
    std::cout << "🚀 STARTING APEX INSTITUTIONAL HFT DAEMON v108.0 (NETWORK LAYER)\n";
    std::cout << "=================================================================\n";

    std::vector<double> latencies;
    latencies.reserve(1000);

    std::thread producer(exchangeProducerThread);
    std::thread consumer(hftConsumerThread, std::ref(latencies));

    producer.join();
    consumer.join();

    if (!latencies.empty()) {
        std::sort(latencies.begin(), latencies.end());
        double min_lat = latencies.front();
        double max_lat = latencies.back();
        double avg_lat = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
        double p99_lat = latencies[static_cast<size_t>(latencies.size() * 0.99)];

        std::cout << "\n=================================================================\n";
        std::cout << "🌐 [NETWORK TRANSPORT & LATENCY PERFORMANCE REPORT (1,000 Ticks)]:\n";
        std::cout << "⚡ Minimum Latency    : " << std::fixed << std::setprecision(2) << min_lat << " ns\n";
        std::cout << "⚡ Average Latency    : " << avg_lat << " ns\n";
        std::cout << "⚡ p99.0 Latency      : " << p99_lat << " ns\n";
        std::cout << "⚡ Maximum Latency    : " << max_lat << " ns\n";
        std::cout << "=================================================================\n";
    }

    return 0;
}