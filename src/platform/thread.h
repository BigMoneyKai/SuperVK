#pragma once
#include "defines.h"

class Thread {
public:
    using EntryPoint = void(*)(void*);
    Thread() = default;
    ~Thread();

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    void start(EntryPoint fn, void* userdata);
    void join();
    b32 joinable() const;

private:
    void* m_handle{nullptr};
};

class Mutex {
public:
    Mutex();
    ~Mutex();

    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

    void lock();
    void unlock();
    bool tryLock();

private:
    void* m_handle{nullptr};

};

class ConditionVariable {
public:
    ConditionVariable();
    ~ConditionVariable();

    void wait(Mutex& mutex);
    void notifyOne();
    void notifyAll();

private:
    void* m_handle{nullptr};
};

