#pragma once
#include "timer.hpp"
#include "stats.hpp"
#include "utils.hpp"
#include <numa.h>
#include <vector>

void run_numa_bench(HighResTimer& timer) {
    std::cout << "\n=== NUMA MEMORY BENCHMARK ===" << std::endl;

    if (numa_available() < 0) {
        std::cout << "NUMA not available on this system." << std::endl;
        return;
    }

    int num_nodes = numa_max_node() + 1;
    if (num_nodes < 2) {
        std::cout << "Only 1 NUMA node detected. Skipping remote test." << std::endl;
        return;
    }

    // Allocate 10MB on Node 0
    size_t size = 10 * 1024 * 1024;
    void* mem = numa_alloc_onnode(size, 0);
    if (!mem) return;
    
    memset(mem, 1, size); // Trigger page faults to actually allocate physical RAM
    volatile char* data = (volatile char*)mem;

    auto measure_access = [&](int cpu_pin_node, const std::string& label) {
        // Run in a thread to pin execution
        std::thread t([&]() {
            // Pin to a CPU on the target node
            // This is a simplification; realistically we find the cpumask for the node
            // assuming cpu 0 is node 0, and cpu X is node 1 (system dependent)
            // For robustness, we use numa_run_on_node
            numa_run_on_node(cpu_pin_node);

            uint64_t start = timer.rdtsc();
            for (size_t i = 0; i < size; i += 64) { // Stride access
                char v = data[i];
                do_not_optimize(v);
            }
            uint64_t end = timer.rdtscp();

            double total_ns = timer.cycles_to_ns(end - start);
            double per_access = total_ns / (size / 64);
            
            std::cout << std::left << std::setw(25) << label 
                      << " | Avg: " << per_access << " ns" << std::endl;
        });
        t.join();
    };

    measure_access(0, "Local (Node 0 -> Node 0)");
    measure_access(1, "Remote (Node 1 -> Node 0)");

    numa_free(mem, size);
}