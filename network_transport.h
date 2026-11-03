#ifndef NETWORK_TRANSPORT_H
#define NETWORK_TRANSPORT_H

#include <iostream>
#include <cstring>
#include <cstdint>

// 📦 Institutional Raw Binary Packet Structure (Packed to eliminate padding delays)
#pragma pack(push, 1)
struct RawBinaryMarketPacket {
    uint64_t magic_header; // e.g., 0x41504558 ("APEX" protocol tag)
    uint64_t sequence_id;
    double bids[5];
    double asks[5];
};
#pragma pack(pop)

class UDPNetworkTransportSimulator {
public:
    // ⚡ Zero-Copy Raw Byte Parsing (Kernel-Bypass Simulation)
    static bool parseRawPacket(const char* raw_buffer, size_t length, MarketTick& out_tick) {
        if (length < sizeof(RawBinaryMarketPacket)) {
            return false; // Drop malformed or incomplete network packets instantly
        }

        // Direct memory mapping (Zero-copy type punning for microsecond speed)
        const RawBinaryMarketPacket* packet = reinterpret_cast<const RawBinaryMarketPacket*>(raw_buffer);

        // Verify protocol magic header
        if (packet->magic_header != 0x41504558) {
            return false; 
        }

        out_tick.sequence_id = packet->sequence_id;
        for(int i = 0; i < 5; ++i) {
            out_tick.bids[i] = packet->bids[i];
            out_tick.asks[i] = packet->asks[i];
        }

        return true;
    }

    // 🌐 Simulate Exchange UDP Multicast Stream Packet Generator
    static size_t createSimulatedExchangePacket(uint64_t seq, const double* b, const double* a, char* out_buffer) {
        RawBinaryMarketPacket packet;
        packet.magic_header = 0x41504558; // "APEX"
        packet.sequence_id = seq;
        for(int i = 0; i < 5; ++i) {
            packet.bids[i] = b[i];
            packet.asks[i] = a[i];
        }
        std::memcpy(out_buffer, &packet, sizeof(RawBinaryMarketPacket));
        return sizeof(RawBinaryMarketPacket);
    }
};

#endif // NETWORK_TRANSPORT_H