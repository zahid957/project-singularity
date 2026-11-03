#ifndef FPGA_ACCELERATOR_H
#define FPGA_ACCELERATOR_H

#include <cmath>

class FPGAHardwareAccelerator {
public:
    // Simulated FPGA Sub-Nanosecond Pipeline
    inline double accelerateTensorEnergy(const double* bids, const double* asks, int depth) const {
        double energy = 0.0;
        for(int i = 0; i < depth; ++i) {
            energy += std::pow(bids[i], 3) - std::pow(asks[i], 3);
        }
        return energy;
    }
};

#endif // FPGA_ACCELERATOR_H