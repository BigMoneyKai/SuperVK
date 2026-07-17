#include "worker.h"
#include "job_system.h"

static void worker_entry(void* arg) {
    auto* worker = static_cast<Worker*>(arg);
    worker->run();
}

void Worker::run() {
    while (system->m_running) {
        Job job;
        if (system->m_jobQueue.pop_front(job)) {
            job.execute();
        }
    }
}

void Worker::steal() {
    Job job;
    if (system->m_jobQueue.try_pop(job))
        job.execute();
}
