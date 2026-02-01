#pragma once
#include "timer.hpp"
#include "stats.hpp"
#include "utils.hpp"
#include <vector>
#include <random>
#include <numeric>

// 64-byte aligned node to fill a cache line
struct alignas(64) Node {
    Node* next;
};

void run_cache_bench(HighResTimer& timer) {
    std::cout << "\n=== CACHE HIERARCHY BENCHMARK ===" << std::endl;

    // Sizes: 32KB, 256KB, 1MB, 8MB, 64MB
    std::vector<size_t> sizes = {
        32 * 1024, 
        256 * 1024, 
        1 * 1024 * 1024, 
        8 * 1024 * 1024, 
        64 * 1024 * 1024 
    };

    for (size_t size : sizes) {
        size_t num_nodes = size / sizeof(Node);
        std::vector<Node> buffer(num_nodes);
        
        // Initialize indices and shuffle to create random pointer chasing
        std::vector<int> indices(num_nodes);
        std::iota(indices.begin(), indices.end(), 0);
        
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(indices.begin(), indices.end(), g);

        // Link nodes based on shuffled indices
        for (size_t i = 0; i < num_nodes - 1; ++i) {
            buffer[indices[i]].next = &buffer[indices[i+1]];
        }
        buffer[indices[num_nodes-1]].next = &buffer[indices[0]]; // Close loop

        // Traverse
        Node* current = &buffer[indices[0]];
        const int STEPS = 1000000; // 1M accesses
        
        uint64_t start = timer.rdtsc();
        for (int i = 0; i < STEPS; ++i) {
            current = current->next;
            // Unroll slightly to reduce loop overhead? 
            // Actually, keep it simple for generic profiler.
        }
        uint64_t end = timer.rdtscp();
        
        do_not_optimize(current);

        double total_ns = timer.cycles_to_ns(end - start);
        double per_access = total_ns / STEPS;

        std::cout << "Size: " << std::right << std::setw(8) << (size/1024) << " KB" 
                  << " | Latency: " << per_access << " ns" << std::endl;
    }
}