#ifndef KERNEL_BYPASS_H
#define KERNEL_BYPASS_H

#include <iostream>
#include <cstdint>

class KernelBypassNICSimulator {
public:
    // Simulates Zero-Copy Direct Memory Access (DMA) from NIC ring buffer (< 50 ns)
    inline bool pollZeroCopyPacket(uint8_t* out_buffer, size_t& out_len) {
        out_len = 64; 
        return true;
    }
};

#endif // KERNEL_BYPASS_H