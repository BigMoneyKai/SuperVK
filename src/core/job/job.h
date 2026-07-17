#pragma once

#include "defines.h"

#include <atomic>

using JobFunc = void (*)(void*);

struct alignas(64) JobCounter {
    std::atomic<i32> value{0};
};

struct alignas(64) Job {
    JobFunc     func{nullptr};
    void*       params{nullptr};
    JobCounter* counter{nullptr};         // user counter (optional)
    JobCounter* global_counter{nullptr};  // always set by JobSystem::submit()
    i64         id{-1};

    void execute();
};
