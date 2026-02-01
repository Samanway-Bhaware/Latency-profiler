#include "core/timer.h"
#include <windows.h>

static double g_ticks_per_ns = 0.0;

void core::Timer::init() {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    g_ticks_per_ns = (double)freq.QuadPart / 1000000000.0;
}

uint64_t core::Timer::now_ticks() {
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return count.QuadPart;
}

uint64_t core::Timer::ticks_to_ns(uint64_t ticks) {
    return (uint64_t)(ticks / g_ticks_per_ns);
}