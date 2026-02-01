#pragma once
#include "core/timer.h"
#include "core/stats.h"
#include <vector>
#include <random>
#include <numeric>

struct Node {
    Node* next;
    uint64_t padding[7]; // Ensure 64-byte cache line alignment
};

void run_cache_benchmark() {
    std::cout << "\n--- Cache / Memory Latency ---\n";
    
    // Sizes: 32KB, 256KB, 1MB, 8MB, 64MB, 256MB
    std::vector<size_t> sizes = {
        32 * 1024, 
        256 * 1024, 
        1024 * 1024, 
        8 * 1024 * 1024, 
        64 * 1024 * 1024,
        256 * 1024 * 1024
    };

    for (auto size_bytes : sizes) {
        size_t num_nodes = size_bytes / sizeof(Node);
        std::vector<Node> nodes(num_nodes);
        
        // Create random permutation indices to defeat prefetcher
        std::vector<size_t> indices(num_nodes);
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), std::mt19937{std::random_device{}()});

        // Link nodes based on shuffled indices
        for (size_t i = 0; i < num_nodes - 1; ++i) {
            nodes[indices[i]].next = &nodes[indices[i+1]];
        }
        nodes[indices[num_nodes-1]].next = &nodes[indices[0]]; // Circular

        // Warmup
        Node* curr = &nodes[0];
        for (size_t i = 0; i < 100000; ++i) curr = curr->next;

        // Measurement
        const size_t iterations = 1000000;
        auto start = core::Timer::now_ticks();
        
        for (size_t i = 0; i < iterations; ++i) {
            curr = curr->next;
        }
        
        auto end = core::Timer::now_ticks();
        
        double total_ns = (double)core::Timer::ticks_to_ns(end - start);
        double per_access_ns = total_ns / iterations;

        std::string label = "Size: " + std::to_string(size_bytes / 1024) + " KB";
        if (size_bytes >= 1024*1024) label = "Size: " + std::to_string(size_bytes / (1024*1024)) + " MB";
        
        std::cout << std::left << std::setw(25) << label 
                  << " | Latency: " << std::setw(8) << per_access_ns << " ns/access\n";
    }
}