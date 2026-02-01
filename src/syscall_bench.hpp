#pragma once
#include "timer.hpp"
#include "stats.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <sys/syscall.h>
#include <vector>

void run_syscall_bench(HighResTimer& timer) {
    const int ITERATIONS = 1000000;
    std::vector<double> latencies;
    latencies.reserve(ITERATIONS);

    std::cout << "\n=== SYSCALL BENCHMARK ===" << std::endl;

    // 1. getpid (Cached mostly in userspace by glibc, but we use syscall directly to measure kernel entry)
    for (int i = 0; i < ITERATIONS; ++i) {
        uint64_t start = timer.rdtsc();
        syscall(SYS_getpid);
        uint64_t end = timer.rdtscp();
        latencies.push_back(timer.cycles_to_ns(end - start));
    }
    Statistics::print("syscall(getpid)", Statistics::compute(latencies));

    // 2. read (0 bytes from stdin - minimal overhead)
    latencies.clear();
    char buf[1];
    for (int i = 0; i < ITERATIONS; ++i) {
        uint64_t start = timer.rdtsc();
        read(0, buf, 0); 
        uint64_t end = timer.rdtscp();
        latencies.push_back(timer.cycles_to_ns(end - start));
    }
    Statistics::print("read(0 bytes)", Statistics::compute(latencies));
}