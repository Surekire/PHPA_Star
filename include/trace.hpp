#pragma once
#include <atomic>
#include <thread>
#include <functional>
#include <memory>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <tracy/Tracy.hpp>

struct alignas(32) event {
    uint64_t timestamp;  
    uint32_t thread_id;  
    uint32_t type;       
    uint64_t data[2];    
};
static_assert(sizeof(event) == 32);

struct spsc_queue {
    static constexpr size_t CAPACITY = 1ULL << 15;
    static constexpr size_t MASK     = CAPACITY - 1;

    // Cache-Line 1: Producer
    std::atomic<uint64_t> head{0};
    uint64_t              tail_cache{0};
    char                  pad0[64 - 2*sizeof(uint64_t)];    //Prevents Cache-Line Invalidations

    // Cache-Line 2: Worker
    std::atomic<uint64_t> tail{0};
    char                  pad1[64 - sizeof(uint64_t)];      //Prevents Buffer Cache-Line Invalidations

    std::unique_ptr<event[]> ring{std::make_unique<event[]>(CAPACITY)};

    bool push(const event& e) noexcept {
        uint64_t h = head.load(std::memory_order_relaxed);
        if (__builtin_expect(h - tail_cache >= CAPACITY, 0)) {
            tail_cache = tail.load(std::memory_order_acquire);
            if (h - tail_cache >= CAPACITY) return false; // Worker holt nach
        }
        ring[h & MASK] = e;
        head.store(h + 1, std::memory_order_release);
        return true;
    }

    size_t pop(event* out, size_t max) noexcept {
        ZoneScopedN("pop");
        uint64_t t = tail.load(std::memory_order_relaxed);
        uint64_t h = head.load(std::memory_order_acquire);
        size_t available = static_cast<size_t>(h - t);
        size_t count = available < max ? available : max;
        for (size_t i = 0; i < count; ++i)
            out[i] = ring[(t + i) & MASK];
        tail.store(t + count, std::memory_order_release);
        return count;
    }

    size_t size() noexcept {
        ZoneScopedN("size");
        return static_cast<size_t>(
            head.load(std::memory_order_acquire) -
            tail.load(std::memory_order_relaxed));
    }
};

class trace {
public:
    using write_fn = std::function<void(const event*, size_t)>;

    explicit trace(write_fn fn);
    ~trace();

    void add_event(const event& e) noexcept;
    void flush();

private:
    spsc_queue& get_local_queue() noexcept;
    void        worker_loop();

    std::mutex                               reg_mutex_;
    std::vector<std::unique_ptr<spsc_queue>> queues_;

    static thread_local spsc_queue* local_queue_;

    write_fn                writer_;
    std::thread             worker_;
    std::mutex              mutex_;
    std::condition_variable cv_;
    bool                    running_{true};
    bool                    flush_requested_{false};
};