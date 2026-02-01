#include <iostream>
#include <string>
#include <vector>
#include "core/timer.h"
#include "core/cpu_affinity.h"
#include "benchmarks/syscall/syscall_bench.h"
#include "benchmarks/cache/cache_bench.h"
#include "benchmarks/context/context_bench.h"
#include "benchmarks/memory/memory_bench.h"

void print_usage() {
    std::cout << "Usage: profiler [syscall|cache|context|memory|all] [--core N]\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    core::Timer::init();

    std::string mode = argv[1];
    int core_id = -1;

    // Simple arg parsing
    for(int i=2; i<argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--core" && i+1 < argc) {
            core_id = std::stoi(argv[++i]);
        }
    }

    if (core_id >= 0) {
        std::cout << "[Info] Pinning to Core: " << core_id << std::endl;
        core::pin_thread_to_core(core_id);
    }

    if (mode == "syscall" || mode == "all") run_syscall_benchmark();
    if (mode == "cache" || mode == "all")   run_cache_benchmark();
    if (mode == "context" || mode == "all") run_context_benchmark();
    if (mode == "memory" || mode == "all")  run_memory_benchmark();

    return 0;
}