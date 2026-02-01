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
    double mean;
    double p50;
    double p95;
    double p99;
};

class Stats {
public:
    static StatsResult calculate(std::vector<double>& samples) {
        if (samples.empty()) return {};

        std::sort(samples.begin(), samples.end());

        double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
        double mean = sum / samples.size();

        return {
            samples.front(),
            samples.back(),
            mean,
            percentile(samples, 0.50),
            percentile(samples, 0.95),
            percentile(samples, 0.99)
        };
    }

    static void print(const std::string& label, const StatsResult& r) {
        std::cout << std::left << std::setw(25) << label
                  << " | Avg: " << std::setw(8) << r.mean
                  << " | p50: " << std::setw(8) << r.p50
                  << " | p95: " << std::setw(8) << r.p95
                  << " | p99: " << std::setw(8) << r.p99
                  << " (ns)\n";
    }

private:
    static double percentile(const std::vector<double>& sorted_data, double p) {
        if (sorted_data.empty()) return 0.0;
        size_t index = static_cast<size_t>(std::ceil(p * sorted_data.size())) - 1;
        return sorted_data[std::max(size_t(0), std::min(index, sorted_data.size() - 1))];
    }
};