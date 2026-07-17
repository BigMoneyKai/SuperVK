#include "job_system.h"

static void worker_entry(void* arg) {
    auto* worker = static_cast<Worker*>(arg);
    worker->run();
}

void JobSystem::init(u32 thread_count, Allocator* allocator) {
    m_allocator = allocator;

    if (thread_count == 0) {
        u32 hc = Thread::hardware_concurrency();
        thread_count = hc > 1 ? hc - 1 : 1;
    }

    m_running      = true;
    m_thread_count = thread_count;

    m_workers = static_cast<Worker*>(
        allocator->allocate(sizeof(Worker) * thread_count, alignof(Worker)));

    for (u32 i = 0; i < thread_count; i++) {
        Worker* w    = &m_workers[i];
        w->id        = i;
        w->system    = this;
        w->thread.start(worker_entry, w);
    }
}

void JobSystem::destroy() {
    m_running = false;
    m_jobQueue.destroy();

    for (u32 i = 0; i < m_thread_count; i++)
        m_workers[i].thread.join();

    m_allocator->deallocate(m_workers, sizeof(Worker) * m_thread_count);
    m_workers     = nullptr;
    m_thread_count = 0;
}

JobSystem::~JobSystem() {
    if (m_running)
        destroy();
}

void JobSystem::submit(const Job& job) {
    m_global_counter.value.fetch_add(1, std::memory_order_acq_rel);
    Job j = job;
    j.global_counter = &m_global_counter;
    m_jobQueue.push_back(std::move(j));
}

void JobSystem::submit(Job&& job) {
    m_global_counter.value.fetch_add(1, std::memory_order_acq_rel);
    Job j = std::move(job);
    j.global_counter = &m_global_counter;
    m_jobQueue.push_back(std::move(j));
}

void JobSystem::submit(JobFunc fn, void* data, JobCounter* counter) {
    m_global_counter.value.fetch_add(1, std::memory_order_acq_rel);

    Job job;
    job.func           = fn;
    job.params         = data;
    job.counter        = counter;
    job.global_counter = &m_global_counter;
    m_jobQueue.push_back(std::move(job));
}

void JobSystem::wait(JobCounter* counter, i32 target) {
    while (counter->value.load(std::memory_order_acquire) != target) {
        Job job;
        if (m_jobQueue.try_pop(job)) {
            job.execute();
        } else {
            Thread::yield();
        }
    }
}

void JobSystem::wait_all() {
    wait(&m_global_counter, 0);
}
