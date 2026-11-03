// (Upar wala poora code yahan paste ho jayega agar aap direct terminal cat command use kar rahe hain)
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <thread>
#include <atomic>

struct MarketTick {
    uint64_t sequence_id;
    double bids[5];
    double asks[5];
    bool valid;
};

class ApexUltimateEngine {
private:
    std::atomic<bool> running{true};
    std::atomic<uint64_t> processed_ticks{0};

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
        std::cout << "📈 Buyer Pressure Probability: " << buyer_probability << "%\n";
        std::cout << "📉 Seller Pressure Probability: " << seller_probability << "%\n";
        
        if (buyer_probability > 58.0) {
            std::cout << "🚀 [SIGNAL] BULLISH EDGE DETECTED: Executing Buy Order Edge\n";
        } else if (seller_probability > 58.0) {
            std::cout << "⚠️ [SIGNAL] BEARISH EDGE DETECTED: Executing Sell Order Edge\n";
        } else {
            std::cout << "⚖️ [SIGNAL] NEUTRAL MARKET: Holding Position\n";
        }
    }

    void simulateDataStream() {
        double mock_bids[5] = {150.2, 150.1, 150.0, 149.8, 149.5};
        double mock_asks[5] = {150.5, 150.6, 150.8, 151.0, 151.2};

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < 5; ++i) {
            std::cout << "\n--- Processing Tick Sequence ID: " << (i + 1) << " ---\n";
            evaluateMarketPressure(mock_bids, mock_asks, 5);
            processed_ticks.fetch_add(1, std::memory_order_relaxed);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::cout << "\n=============================================================\n";
        std::cout << "⚡ Total Processed Ticks: " << processed_ticks.load() << "\n";
        std::cout << "⚡ Total Execution Time (5 Iterations): " << duration << " ns\n";
        std::cout << "⚡ Average Latency per Tick: " << (duration / 5.0) << " ns\n";
    }
};

int main() {
    std::cout << "🏛️ PROJECT SINGULARITY v105.0: ULTIMATE MULTI-THREADED ENGINE\n";
    std::cout << "=============================================================\n";
    
    ApexUltimateEngine engine;
    std::thread worker(&ApexUltimateEngine::simulateDataStream, &engine);
    worker.join();
    
    return 0;
}