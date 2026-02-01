#pragma once
#include "core/timer.h"
#include "core/stats.h"
#include <unistd.h>
#include <vector>
#include <fcntl.h>

void run_syscall_benchmark() {
    std::cout << "\n--- Syscall Benchmarks ---\n";
    const int N = 100000;
    std::vector<double> samples;
    samples.reserve(N);

    // GETPID
    for(int i=0; i<N; ++i) {
        auto start = core::Timer::now_ticks();
        volatile pid_t p = getpid(); (void)p;
        auto end = core::Timer::now_ticks();
        samples.push_back((double)core::Timer::ticks_to_ns(end - start));
    }
    Stats::print("getpid()", Stats::calculate(samples));

    // READ (1 byte /dev/zero)
    samples.clear();
    int fd = open("/dev/zero", O_RDONLY);
    char buf[1];
    for(int i=0; i<N; ++i) {
        auto start = core::Timer::now_ticks();
        volatile ssize_t s = read(fd, buf, 1); (void)s;
        auto end = core::Timer::now_ticks();
        samples.push_back((double)core::Timer::ticks_to_ns(end - start));
    }
    close(fd);
    Stats::print("read(1B /dev/zero)", Stats::calculate(samples));
}