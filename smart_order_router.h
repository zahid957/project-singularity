#ifndef SMART_ORDER_ROUTER_H
#define SMART_ORDER_ROUTER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct ExchangeQuote {
    std::string exchange_name;
    double best_bid;
    double best_ask;
};

class SmartOrderRouter {
public:
    static std::string routeBestExecution(const std::string& side, const std::vector<ExchangeQuote>& quotes) {
        if (quotes.empty()) return "LOCAL_EXCHANGE";

        if (side == "BUY") {
            auto best_it = std::min_element(quotes.begin(), quotes.end(), [](const ExchangeQuote& a, const ExchangeQuote& b) {
                return a.best_ask < b.best_ask;
            });
            std::cout << "🌐 [SOR ROUTING] Best BUY routed to " << best_it->exchange_name 
                      << " @ Ask: " << best_it->best_ask << "\n";
            return best_it->exchange_name;
        } else {
            auto best_it = std::max_element(quotes.begin(), quotes.end(), [](const ExchangeQuote& a, const ExchangeQuote& b) {
                return a.best_bid < b.best_bid;
            });
            std::cout << "🌐 [SOR ROUTING] Best SELL routed to " << best_it->exchange_name 
                      << " @ Bid: " << best_it->best_bid << "\n";
            return best_it->exchange_name;
        }
    }
};

#endif // SMART_ORDER_ROUTER_H