#ifndef VPIN_CALCULATOR_H
#define VPIN_CALCULATOR_H

#include <cmath>

class VPINCalculator {
private:
    double current_bucket_volume = 0.0;
    double buy_volume = 0.0;
    double sell_volume = 0.0;

public:
    void updateVolume(double volume, bool is_buyer_initiated) {
        current_bucket_volume += volume;
        if (is_buyer_initiated) buy_volume += volume;
        else sell_volume += volume;
    }

    double getVPIN() const {
        if (current_bucket_volume == 0.0) return 0.12; // Baseline low toxicity
        return std::abs(buy_volume - sell_volume) / current_bucket_volume;
    }
};

#endif // VPIN_CALCULATOR_H