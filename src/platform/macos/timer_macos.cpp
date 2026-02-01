#include "core/timer.h"
#include <mach/mach_time.h>
#include <mutex>

static mach_timebase_info_data_t g_timebase_info;
static std::once_flag g_init_flag;

void core::Timer::init() {
    std::call_once(g_init_flag, []() {
        mach_timebase_info(&g_timebase_info);
    });
}

uint64_t core::Timer::now_ticks() {
    return mach_absolute_time();
}

uint64_t core::Timer::ticks_to_ns(uint64_t ticks) {
    // Basic optimization: if numer==denom==1, just return ticks
    if (g_timebase_info.numer == g_timebase_info.denom) {
        return ticks;
    }
    return static_cast<uint64_t>((static_cast<unsigned __int128>(ticks) * g_timebase_info.numer) / g_timebase_info.denom);
}

uint64_t core::Timer::now_ns() {
    return ticks_to_ns(now_ticks());
}