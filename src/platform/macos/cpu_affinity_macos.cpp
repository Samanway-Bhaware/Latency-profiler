#include "core/cpu_affinity.h"
#include <mach/thread_act.h>
#include <mach/thread_policy.h>
#include <mach/mach.h>
#include <iostream>

void core::pin_thread_to_core(int core_id) {
    thread_affinity_policy_data_t policy = { static_cast<integer_t>(core_id) };
    thread_port_t mach_thread = mach_thread_self();
    
    kern_return_t ret = thread_policy_set(
        mach_thread,
        THREAD_AFFINITY_POLICY,
        (thread_policy_t)&policy,
        THREAD_AFFINITY_POLICY_COUNT
    );

    if (ret != KERN_SUCCESS) {
        std::cerr << "Warning: Failed to set thread affinity for core " << core_id << std::endl;
    }
    
    // Release the port
    mach_port_deallocate(mach_task_self(), mach_thread);
}