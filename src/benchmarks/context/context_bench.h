#pragma once
#include "core/timer.h"
#include "core/stats.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <sys/wait.h>

void run_context_benchmark() {
    std::cout << "\n--- Context Switch Cost ---\n";

    // 1. Thread Ping-Pong
    std::mutex mtx;
    std::condition_variable cv; 
    bool ready = false;
    const int iter = 10000;

    auto t_start = core::Timer::now_ticks();
    
    std::thread t1([&](){
        for(int i=0; i<iter; ++i) {
            std::unique_lock<std::mutex> lk(mtx);
            cv.wait(lk, [&]{ return ready; });
            ready = false;
            lk.unlock();
            cv.notify_one();
        }
    });

    std::thread t2([&](){
        for(int i=0; i<iter; ++i) {
            std::unique_lock<std::mutex> lk(mtx);
            ready = true;
            lk.unlock();
            cv.notify_one();
            
            lk.lock();
            cv.wait(lk, [&]{ return !ready; });
        }
    });

    t1.join();
    t2.join();
    
    auto t_end = core::Timer::now_ticks();
    double ns = (double)core::Timer::ticks_to_ns(t_end - t_start);
    std::cout << "Thread Switch (Mutex/CV)  | " << (ns / (iter * 2)) << " ns/switch\n";

    // 2. Process Ping-Pong (Pipes)
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) return;

    char byte = 'x';
    auto p_start = core::Timer::now_ticks();

    pid_t pid = fork();
    if (pid == 0) {
        // Child
        for(int i=0; i<iter; ++i) {
            read(pipe1[0], &byte, 1);
            write(pipe2[1], &byte, 1);
        }
        exit(0);
    } else {
        // Parent
        for(int i=0; i<iter; ++i) {
            write(pipe1[1], &byte, 1);
            read(pipe2[0], &byte, 1);
        }
        wait(NULL);
    }

    auto p_end = core::Timer::now_ticks();
    ns = (double)core::Timer::ticks_to_ns(p_end - p_start);
    std::cout << "Process Switch (Pipe)     | " << (ns / (iter * 2)) << " ns/switch\n";
}