#include "job_queue.h"

void JobQueue::push_back(const Job& job) {
    m_mutex.lock();
    m_jobs.push_back(job);
    m_mutex.unlock();
    m_cv.notify_one();
}

void JobQueue::push_back(Job&& job) {
    m_mutex.lock();
    m_jobs.push_back(std::move(job));
    m_mutex.unlock();
    m_cv.notify_one();
}

b32 JobQueue::pop_front(Job& job) {
    m_mutex.lock();
    while (m_jobs.empty() && !m_destroyed)
        m_cv.wait(m_mutex);

    if (m_destroyed && m_jobs.empty()) {
        m_mutex.unlock();
        return false;
    }

    job = m_jobs.front();
    m_jobs.pop_front();
    m_mutex.unlock();
    return true;
}

b32 JobQueue::try_pop(Job& job) {
    m_mutex.lock();
    if (m_jobs.empty()) {
        m_mutex.unlock();
        return false;
    }
    job = m_jobs.front();
    m_jobs.pop_front();
    m_mutex.unlock();
    return true;
}

void JobQueue::destroy() {
    m_mutex.lock();
    m_destroyed = true;
    m_mutex.unlock();
    m_cv.notify_all();
}
