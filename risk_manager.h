#ifndef RISK_MANAGER_H
#define RISK_MANAGER_H

#include <iostream>
#include <cmath>

class RiskManager {
private:
    double max_allowed_drawdown;
    double current_pnl;
    int max_consecutive_losses;
    int current_consecutive_losses;
    bool trading_locked;

public:
    RiskManager(double max_dd = 50.0, int max_losses = 3) 
        : max_allowed_drawdown(max_dd), current_pnl(0.0), 
          max_consecutive_losses(max_losses), current_consecutive_losses(0), 
          trading_locked(false) {}

    bool validateTradeExecution(double proposed_risk_amount) {
        if (trading_locked) {
            std::cout << "🛑 [RISK MANAGER] TRADING LOCKED DUE TO SAFETY LIMIT BREACH!\n";
            return false;
        }

        if (std::abs(current_pnl) >= max_allowed_drawdown) {
            trading_locked = true;
            std::cout << "🚨 [RISK ALERT] Max Drawdown reached! Halting execution.\n";
            return false;
        }

        return true;
    }

    void recordTradeResult(bool is_win, double pnl_delta) {
        if (is_win) {
            current_pnl += pnl_delta;
            current_consecutive_losses = 0; // Reset streak
        } else {
            current_pnl -= pnl_delta;
            current_consecutive_losses++;
            if (current_consecutive_losses >= max_consecutive_losses) {
                trading_locked = true;
                std::cout << "⚠️ [RISK ALERT] Max consecutive losses (" << max_consecutive_losses << ") hit. Locking risk engine.\n";
            }
        }
    }

    void resetRiskState() {
        trading_locked = false;
        current_pnl = 0.0;
        current_consecutive_losses = 0;
    }

    bool isLocked() const { return trading_locked; }
    double getCurrentPnL() const { return current_pnl; }
};

#endif // RISK_MANAGER_H