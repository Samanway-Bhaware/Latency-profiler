#include "core/cpu_affinity.h"
#include <windows.h>
#include <iostream>

void core::pin_thread_to_core(int core_id) {
    HANDLE thread = GetCurrentThread();
    DWORD_PTR mask = (1ULL << core_id);
    
    if (SetThreadAffinityMask(thread, mask) == 0) {
        std::cerr << "Failed to set affinity mask for core " << core_id << "\n";
    }
}