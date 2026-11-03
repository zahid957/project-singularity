#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <iostream>
#include <string>

class TelemetryExporter {
public:
    static void exportMetricsToJson(uint64_t total_trades, double win_rate, double pnl, double avg_latency_ns) {
        std::cout << "\n📊 [TELEMETRY EXPORTER - PROMETHEUS/JSON METRICS]\n";
        std::cout << "{\n";
        std::cout << "  \"daemon\": \"ApexInstitutionalEngine_v108.0\",\n";
        std::cout << "  \"total_trades\": " << total_trades << ",\n";
        std::cout << "  \"win_rate_pct\": " << win_rate << ",\n";
        std::cout << "  \"cumulative_pnl\": " << pnl << ",\n";
        std::cout << "  \"avg_latency_ns\": " << avg_latency_ns << "\n";
        std::cout << "}\n";
        std::cout << "-----------------------------------------------------------------\n";
    }
};

#endif // TELEMETRY_H