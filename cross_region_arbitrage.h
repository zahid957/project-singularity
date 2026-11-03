#ifndef CROSS_REGION_ARBITRAGE_H
#define CROSS_REGION_ARBITRAGE_H

#include <string>
#include <algorithm>

class CrossRegionArbitrageManager {
public:
    std::string evaluateCrossExchangeArbitrage(double local_price, double remote_price, double threshold) {
        if (remote_price - local_price > threshold) {
            return "EXECUTE_NY4_TY3_ARBITRAGE";
        }
        return "HOLD_LOCAL";
    }
};

#endif // CROSS_REGION_ARBITRAGE_H