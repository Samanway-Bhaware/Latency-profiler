#pragma once
#include "core/timer.h"
#include <vector>
#include <cstring>
#include <iostream>

void run_memory_benchmark() {
    std::cout << "\n--- Memory Locality (First Touch vs Hot) ---\n";
    
    const size_t size = 64 * 1024 * 1024; // 64MB
    
    // Allocate raw memory (lazy allocation by OS)
    char* buffer = new char[size]; 

    // Measure First Touch (Page Fault trigger)
    auto start = core::Timer::now_ticks();
    // Stride by 4096 (page size) to trigger faults
    for(size_t i = 0; i < size; i += 4096) {
        buffer[i] = 1; 
    }
    auto end = core::Timer::now_ticks();
    std::cout << "First Touch (Page Faults) | " << core::Timer::ticks_to_ns(end - start) / (size/4096) << " ns/page\n";

    // Measure Hot Access
    start = core::Timer::now_ticks();
    for(size_t i = 0; i < size; i += 4096) {
        buffer[i] = 2;
    }
    end = core::Timer::now_ticks();
    std::cout << "Hot Access (TLB Hit)      | " << core::Timer::ticks_to_ns(end - start) / (size/4096) << " ns/page\n";

    delete[] buffer;
}