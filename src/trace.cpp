#include "trace.hpp"
#include <tracy/Tracy.hpp>

thread_local spsc_queue* trace::local_queue_ = nullptr;

trace::trace(write_fn fn) : writer_(std::move(fn))
{
    worker_ = std::thread(&trace::worker_loop, this);
}

trace::~trace()
{
    ZoneScopedN("destructor")
    flush();
    {
        std::lock_guard lk(mutex_);
        running_ = false;
    }
    cv_.notify_one();
    if (worker_.joinable())
        worker_.join();

    // thread_local zurücksetzen damit der nächste trace
    // eine neue Queue registriert
    local_queue_ = nullptr;
}

spsc_queue& trace::get_local_queue() noexcept
{
    ZoneScopedN("get_local_queue");
    if (__builtin_expect(local_queue_ != nullptr, 1))
        return *local_queue_;

    std::lock_guard lk(reg_mutex_);
    queues_.push_back(std::make_unique<spsc_queue>());
    local_queue_ = queues_.back().get();
    return *local_queue_;
}

void trace::add_event(const event& e_in) noexcept
{
    spsc_queue& q = get_local_queue();
    while (!q.push(e_in))
        __builtin_ia32_pause(); // warten bis Worker Platz macht
}

void trace::flush()
{
    ZoneScopedN("flush");
    {
        std::lock_guard lk(mutex_);
        flush_requested_ = true;
    }
    cv_.notify_one();

    std::unique_lock lk(mutex_);
    cv_.wait(lk, [this] { return !flush_requested_; });
}

void trace::worker_loop()
{
    ZoneScopedN("Worker Loop");
    static constexpr size_t BATCH = 4096;
    event    batch[BATCH];
    uint32_t seq = 0;
    std::vector<spsc_queue*> local_qs;
    int idle_spins = 0;

    while (true) {
        {
            std::lock_guard lk(reg_mutex_);
            if (local_qs.size() != queues_.size()) {
                local_qs.clear();
                for (auto& q : queues_)
                    local_qs.push_back(q.get());
            }
        }

        bool did_work = false;
        for (spsc_queue* q : local_qs) {
            size_t count = q->pop(batch, BATCH);
            if (count > 0) {
                writer_(batch, count);
                did_work = true;
                idle_spins = 0;
            }
        }

        if (!did_work) {
            ZoneScopedN("worker Loop idle");
            ++idle_spins;

            if (idle_spins < 1000) {
                // kurz warten ohne Kernel – CPU hint
                __builtin_ia32_pause();
                continue;
            }

            if (idle_spins < 10000) {
                // etwas länger warten
                std::this_thread::yield();
                continue;
            }

            // wirklich idle – jetzt schlafen
            std::unique_lock lk(mutex_);
            cv_.wait_for(lk, std::chrono::milliseconds(1), [this] {
                return !running_ || flush_requested_;
            });
            idle_spins = 0;

            if (flush_requested_) {
                lk.unlock();
                {
                    std::lock_guard rlk(reg_mutex_);
                    if (local_qs.size() != queues_.size()) {
                        local_qs.clear();
                        for (auto& q : queues_)
                            local_qs.push_back(q.get());
                    }
                }
                for (spsc_queue* q : local_qs) {
                    size_t count;
                    while ((count = q->pop(batch, BATCH)) > 0) {
                        writer_(batch, count);
                    }
                }
                {
                    std::lock_guard lk2(mutex_);
                    flush_requested_ = false;
                }
                cv_.notify_all();
            }

            if (!running_) break;
        }
    }
}