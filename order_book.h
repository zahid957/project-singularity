#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <iostream>
#include <vector>
#include <algorithm>

struct BookLevel {
    double price;
    double volume;
};

class L2OrderBook {
private:
    std::vector<BookLevel> bids;
    std::vector<BookLevel> asks;

public:
    void updateBook(const double* raw_bids, const double* raw_asks, int depth) {
        bids.clear();
        asks.clear();

        for(int i = 0; i < depth; ++i) {
            bids.push_back({raw_bids[i], 1.5 + (i * 0.2)}); // Simulated aggregated liquidity volume
            asks.push_back({raw_asks[i], 1.5 + (i * 0.2)});
        }

        // Sort Bids descending (highest price first)
        std::sort(bids.begin(), bids.end(), [](const BookLevel& a, const BookLevel& b) {
            return a.price > b.price;
        });

        // Sort Asks ascending (lowest price first)
        std::sort(asks.begin(), asks.end(), [](const BookLevel& a, const BookLevel& b) {
            return a.price < b.price;
        });
    }

    double getBestBid() const { return bids.empty() ? 0.0 : bids[0].price; }
    double getBestAsk() const { return asks.empty() ? 0.0 : asks[0].price; }
    
    void printBookSummary() const {
        std::cout << "📚 [LOB RECONSTRUCTION] Best Bid: " << getBestBid() 
                  << " | Best Ask: " << getBestAsk() 
                  << " | Spread: " << (getBestAsk() - getBestBid()) << "\n";
    }
};

#endif // ORDER_BOOK_H