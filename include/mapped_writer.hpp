#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "trace.hpp"

class mapped_writer {
public:
    mapped_writer(const char* path, size_t max_events) 
        : max_events_(max_events)
        , file_size_(max_events * sizeof(event))
    {
        fd_ = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fd_ < 0)
            throw std::runtime_error("open failed");

        // Datei auf maximale Größe vorallokieren
        if (ftruncate(fd_, file_size_) < 0)
            throw std::runtime_error("ftruncate failed");

        ptr_ = (event*)mmap(nullptr, file_size_,
                            PROT_WRITE, MAP_SHARED, fd_, 0);
        if (ptr_ == MAP_FAILED)
            throw std::runtime_error("mmap failed");

        // OS hint: wir schreiben sequenziell
        madvise(ptr_, file_size_, MADV_SEQUENTIAL);
    }

    ~mapped_writer()
    {
        if (ptr_ && ptr_ != MAP_FAILED) {
            msync(ptr_, offset_ * sizeof(event), MS_SYNC);
            munmap(ptr_, file_size_);
        }
        if (fd_ >= 0) {
            // Datei auf tatsächliche Größe kürzen
            ftruncate(fd_, offset_ * sizeof(event));
            close(fd_);
        }
    }

    // kein copy
    mapped_writer(const mapped_writer&)            = delete;
    mapped_writer& operator=(const mapped_writer&) = delete;

    void write(const event* buf, size_t count)
    {
        // sollte nie passieren wenn max_events groß genug
        if (offset_ + count > max_events_)
            count = max_events_ - offset_;

        memcpy(ptr_ + offset_, buf, count * sizeof(event));
        offset_ += count;
    }

    size_t count()     const { return offset_; }
    bool   is_full()   const { return offset_ >= max_events_; }
    void reset()             { offset_ = 0; }

private:
    int    fd_         = -1;
    event* ptr_        = nullptr;
    size_t max_events_ = 0;
    size_t file_size_  = 0;
    size_t offset_     = 0;
};