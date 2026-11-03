#ifndef MARKET_MAKER_H
#define MARKET_MAKER_H

#include <iostream>
#include <cmath>

class MarketMakerStrategy {
private:
    double base_spread;

public:
    MarketMakerStrategy(double spread = 0.10) : base_spread(spread) {}

    void calculatePassiveQuotes(double mid_price, double volatility, double& out_bid, double& out_ask) {
        double dynamic_spread = base_spread * (1.0 + volatility);
        out_bid = mid_price - (dynamic_spread / 2.0);
        out_ask = mid_price + (dynamic_spread / 2.0);

        std::cout << "📈 [MARKET MAKER] Passive Bid Quote: " << out_bid 
                  << " | Passive Ask Quote: " << out_ask 
                  << " (Spread: " << dynamic_spread << ")\n";
    }
};

#endif // MARKET_MAKER_H