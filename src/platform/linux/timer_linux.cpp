#include "core/timer.h"
#include <time.h>

void core::Timer::init() {
    // No initialization needed for clock_gettime
}

uint64_t core::Timer::now_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

uint64_t core::Timer::now_ticks() {
    // On Linux, we often just use ns directly, or rdtsc if you want extreme precision
    return now_ns(); 
}

uint64_t core::Timer::ticks_to_ns(uint64_t ticks) {
    return ticks; // 1:1 mapping if using clock_gettime
}