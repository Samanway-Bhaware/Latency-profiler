#include "core/cpu_affinity.h"
#include <sched.h>
#include <pthread.h>
#include <iostream>

void core::pin_thread_to_core(int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t thread = pthread_self();
    if (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) != 0) {
        std::cerr << "Failed to pin thread to core " << core_id << "\n";
    }
}