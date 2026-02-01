#pragma once
#include "timer.hpp"
#include "stats.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <thread>
#include <atomic>

void run_context_bench(HighResTimer& timer) {
    std::cout << "\n=== CONTEXT SWITCH BENCHMARK ===" << std::endl;
    const int ITERATIONS = 10000;

    // --- Process Context Switch (via Pipes) ---
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) return;

    uint64_t start = timer.rdtsc();
    pid_t pid = fork();

    if (pid == 0) { // Child
        char byte;
        for (int i = 0; i < ITERATIONS; ++i) {
            read(pipe1[0], &byte, 1);
            write(pipe2[1], &byte, 1);
        }
        exit(0);
    } else { // Parent
        char byte = 'x';
        pin_to_core(0); // Pin parent to core 0
        // Ideally child ends up on same core for strict context switch
        // or different core for migration cost. We assume scheduler default here.
        
        for (int i = 0; i < ITERATIONS; ++i) {
            write(pipe1[1], &byte, 1);
            read(pipe2[0], &byte, 1);
        }
        wait(NULL);
        uint64_t end = timer.rdtscp();
        
        double total_ns = timer.cycles_to_ns(end - start);
        // Divide by 2 (round trip) and iterations
        double latency = total_ns / (ITERATIONS * 2);
        
        StatsResult res = {latency, latency, latency, latency, latency, latency};
        Statistics::print("Process Ctx Switch", res);
    }
}