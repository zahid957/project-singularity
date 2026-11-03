#ifndef FIX_PROTOCOL_H
#define FIX_PROTOCOL_H

#include <iostream>
#include <string>

class FIXSessionManager {
private:
    uint64_t msg_seq_num = 1;

public:
    void sendLogon() {
        msg_seq_num++;
    }

    std::string createNewOrderSingle(const std::string& symbol, double price, double qty, const std::string& side) {
        std::string fix_msg = "8=FIX.4.4|35=D|34=" + std::to_string(msg_seq_num++) + 
                              "|55=" + symbol + "|54=" + (side == "BUY" ? "1" : "2") + 
                              "|44=" + std::to_string(price) + "|38=" + std::to_string(qty);
        return fix_msg;
    }
};

#endif // FIX_PROTOCOL_H