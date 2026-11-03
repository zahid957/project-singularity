#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <cmath>
#include <iomanip>

struct MarketTick {
    uint64_t sequence_id;
    double bids[5];
    double asks[5];
};

class ApexInstitutionalEngine {
private:
    double previous_imbalance = 0.0;

public:
    void evaluateAndExecute(const double* bids, const double* asks, int depth) {
        double bid_tensor_energy = 0.0;
        double ask_tensor_energy = 0.0;
        
        for(int i = 0; i < depth; ++i) {
            bid_tensor_energy += std::pow(bids[i], 3) * (depth - i); 
            ask_tensor_energy += std::pow(asks[i], 3) * (depth - i);
        }
        
        double total_energy = bid_tensor_energy + ask_tensor_energy;
        double buyer_prob = (total_energy > 0) ? (bid_tensor_energy / total_energy) * 100.0 : 50.0;
        double seller_prob = (total_energy > 0) ? (ask_tensor_energy / total_energy) * 100.0 : 50.0;
        
        double current_imbalance = buyer_prob - seller_prob;
        double ofi_momentum = current_imbalance - previous_imbalance;
        previous_imbalance = current_imbalance;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "📈 Buyer: " << buyer_prob << "% | 📉 Seller: " << seller_prob 
                  << "% | ⚡ OFI Momentum: " << (ofi_momentum >= 0 ? "+" : "") << ofi_momentum << "%\n";
        
        double mid_price = (bids[0] + asks[0]) / 2.0;
        if (buyer_prob > 58.0 && ofi_momentum > 0) {
            double stop_loss = mid_price - 0.15;
            double take_profit = mid_price + 0.30;
            std::cout << "🚀 [EXECUTION] BUY SIGNAL | Entry: " << mid_price 
                      << " | SL: " << stop_loss << " | TP: " << take_profit << "\n";
        } else if (seller_prob > 58.0 && ofi_momentum < 0) {
            double stop_loss = mid_price + 0.15;
            double take_profit = mid_price - 0.30;
            std::cout << "⚠️ [EXECUTION] SELL SIGNAL | Entry: " << mid_price 
                      << " | SL: " << stop_loss << " | TP: " << take_profit << "\n";
        } else {
            std::cout << "⚖️ [SIGNAL] NEUTRAL / SPREAD EQUILIBRIUM\n";
        }
    }
};

#endif // ENGINE_H