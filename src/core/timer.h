#pragma once
#include <cstdint>

namespace core {
    class Timer {
    public:
        // Initialize hardware timebase (call once at startup)
        static void init();
        
        // Get current timestamp in nanoseconds
        static uint64_t now_ns();
        
        // Convert raw ticks to nanoseconds (for calculating delta later)
        static uint64_t ticks_to_ns(uint64_t ticks);
        
        // Get raw hardware ticks (lower overhead)
        static uint64_t now_ticks();
    };
}