#pragma once
#include <sched.h>
#include <pthread.h>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <iostream>

// Prevent compiler from optimizing away variables
inline void pin_to_core(int core_id) {
#ifdef __APPLE_MACOS__
    // macOS uses thread_policy_set, but for a shim, we can warn and no-op
    static bool warned = false;
    if (!warned) {
        std::cerr << "[WARN] CPU Pinning not supported on macOS. Running unpinned." << std::endl;
        warned = true;
    }
#else
    // Original Linux Code
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        std::cerr << "Warning: Failed to pin thread to core " << core_id << std::endl;
    }
#endif
}