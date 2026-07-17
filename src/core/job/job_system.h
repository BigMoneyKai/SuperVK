#pragma once

#include "defines.h"
#include "core/memory/allocator.h"
#include "job.h"
#include "job_queue.h"
#include "worker.h"

#include <atomic>

class JobSystem {
public:
    JobSystem() = default;
    ~JobSystem();

    JobSystem(const JobSystem&) = delete;
    JobSystem& operator=(const JobSystem&) = delete;

    void init(u32 thread_count = 0, Allocator* allocator = &default_allocator());
    void destroy();

    void submit(const Job& job);
    void submit(Job&& job);
    void submit(JobFunc fn, void* data, JobCounter* counter = nullptr);

    void wait(JobCounter* counter, i32 target);
    void wait_all();

    u32  thread_count() const { return m_thread_count; }
    b32  running()      const { return m_running; }

private:
    friend struct Worker;

    Allocator* m_allocator{nullptr};
    u32        m_thread_count{0};
    b32        m_running{false};
    JobQueue   m_jobQueue;
    JobCounter m_global_counter;
    Worker*    m_workers{nullptr};
};
