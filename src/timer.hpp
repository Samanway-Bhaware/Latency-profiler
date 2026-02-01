#pragma once
#include <cstdint>
#include <time.h>
#include <thread>
#include <chrono>

class HighResTimer {
public:
    HighResTimer() {
        calibrate();
    }

    // Serializing instruction to prevent out-of-order execution leakage
    inline uint64_t rdtsc() {
        unsigned int lo, hi;
        // lfence ensures all previous instructions finish before reading time
        asm volatile("lfence" ::: "memory");
        asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
        return ((uint64_t)hi << 32) | lo;
    }

    inline uint64_t rdtscp() {
        unsigned int lo, hi, aux;
        asm volatile("rdtscp" : "=a" (lo), "=d" (hi), "=c" (aux));
        asm volatile("lfence" ::: "memory");
        return ((uint64_t)hi << 32) | lo;
    }

    // Convert cycles to nanoseconds
    double cycles_to_ns(uint64_t cycles) const {
        return cycles * ns_per_cycle_;
    }

private:
    double ns_per_cycle_ = 0.0;

    void calibrate() {
        // Warmup
        auto start_tsc = rdtsc();
        auto start_sys = std::chrono::steady_clock::now();
        
        // Sleep for 100ms to measure frequency
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
        auto end_tsc = rdtsc();
        auto end_sys = std::chrono::steady_clock::now();

        auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_sys - start_sys).count();
        auto elapsed_cycles = end_tsc - start_tsc;

        ns_per_cycle_ = (double)elapsed_ns / (double)elapsed_cycles;
    }
};
inline uint64_t rdtsc() {
#if defined(__aarch64__)
    // Apple Silicon / ARM64 equivalent
    uint64_t val;
    asm volatile("mrs %0, cntvct_el0" : "=r" (val));
    return val;
#else
    // Standard x86_64
    unsigned int lo, hi;
    asm volatile("lfence" ::: "memory");
    asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
#endif
}