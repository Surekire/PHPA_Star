#include "trace.hpp"
#include "mapped_writer.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tracy/Tracy.hpp>
#include <sys/syscall.h>

inline uint64_t rdtsc() {
    uint32_t lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return (uint64_t(hi) << 32) | lo;
}

int main()
{
    constexpr size_t EVENTS_PER_THREAD = 1;
    constexpr size_t TOTAL_EVENTS      = EVENTS_PER_THREAD * 2;

    mapped_writer mw("trace.bin", TOTAL_EVENTS);

    trace t([&](const event* buf, size_t count) {
        mw.write(buf, count);
    });

    auto t0 = rdtsc();

    std::thread producer1([&]{
        uint32_t tid = (uint32_t)syscall(SYS_gettid);
        for (size_t i = 0; i < EVENTS_PER_THREAD; ++i) {
            event e{rdtsc(), tid, 0, {i, 0}};
            t.add_event(e);
        }
    });

    std::thread producer2([&]{
        uint32_t tid = (uint32_t)syscall(SYS_gettid);
        for (size_t i = 0; i < EVENTS_PER_THREAD; ++i) {
            event e{rdtsc(), tid, 0, {i, 0}};
            t.add_event(e);
        }
    });

    producer1.join();
    producer2.join();

    auto t1 = rdtsc();

    t.flush();

    double seconds = (t1 - t0) / 3.6e9;
    size_t written = mw.count();
    size_t drops   = TOTAL_EVENTS - written;

    std::cout << "written : " << written  << " / " << TOTAL_EVENTS << " events\n";
    std::cout << "drops   : " << drops    << "\n";
    std::cout << "time    : " << seconds  << " s\n";
    std::cout << "rate    : " << double(TOTAL_EVENTS) / seconds / 1e6 << " M events/s\n";
    std::cout << "per thread: " << double(EVENTS_PER_THREAD) / seconds / 1e6 << " M events/s\n";

    return 0;
}