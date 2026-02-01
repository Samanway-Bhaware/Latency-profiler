#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>
#include <iomanip>

struct StatsResult {
    double min;
    double max;
    double p50;
    double p95;
    double p99;
    double mean;
};

class Statistics {
public:
    static StatsResult compute(std::vector<double>& data) {
        if (data.empty()) return {0,0,0,0,0,0};

        // Sort for percentiles
        std::sort(data.begin(), data.end());

        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        size_t n = data.size();

        return {
            data.front(),
            data.back(),
            data[static_cast<size_t>(n * 0.50)],
            data[static_cast<size_t>(n * 0.95)],
            data[static_cast<size_t>(n * 0.99)],
            sum / n
        };
    }

    static void print(const std::string& label, const StatsResult& s) {
        std::cout << std::left << std::setw(25) << label 
                  << " | p50: " << std::fixed << std::setprecision(1) << s.p50 << " ns"
                  << " | p99: " << s.p99 << " ns"
                  << " | Max: " << s.max << " ns" << std::endl;
    }
};