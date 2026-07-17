#include "time.h"

#if defined(SV_PLATFORM_LINUX) || defined(SV_PLATFORM_APPLE)
#include <time.h>
#include <cerrno>

static u64 monotonic_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (u64)ts.tv_sec * 1000000000 + (u64)ts.tv_nsec;
}

void timer_init(Timer* t) {
    t->start_ns = monotonic_ns();
}

void timer_start(Timer* t) {
    t->start_ns = monotonic_ns();
}

void timer_reset(Timer* t) {
    t->start_ns = monotonic_ns();
}

f64 timer_elapsed_ms(const Timer* t) {
    return (f64)(monotonic_ns() - t->start_ns) / 1000000.0;
}

f64 timer_elapsed_us(const Timer* t) {
    return (f64)(monotonic_ns() - t->start_ns) / 1000.0;
}

f64 timer_elapsed_s(const Timer* t) {
    return (f64)(monotonic_ns() - t->start_ns) / 1000000000.0;
}

f64 timer_now_ms(void) {
    return (f64)monotonic_ns() / 1000000.0;
}

void timer_sleep_ms(u64 ms) {
    struct timespec ts;
    ts.tv_sec  = (time_t)(ms / 1000);
    ts.tv_nsec = (long)(ms % 1000) * 1000000;
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
        ;
}

void timer_sleep_us(u64 us) {
    struct timespec ts;
    ts.tv_sec  = (time_t)(us / 1000000);
    ts.tv_nsec = (long)(us % 1000000) * 1000;
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
        ;
}

#elif defined(SV_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static f64 s_frequency = 0.0;

static u64 query_counter(void) {
    LARGE_INTEGER cnt;
    QueryPerformanceCounter(&cnt);
    return (u64)cnt.QuadPart;
}

static f64 counter_to_ns(u64 ticks) {
    if (s_frequency == 0.0) {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        s_frequency = (f64)freq.QuadPart;
    }
    return (f64)ticks / s_frequency * 1000000000.0;
}

static u64 monotonic_ns(void) {
    return (u64)counter_to_ns(query_counter());
}

void timer_init(Timer* t) {
    t->start_ns = monotonic_ns();
}

void timer_start(Timer* t) {
    t->start_ns = monotonic_ns();
}

void timer_reset(Timer* t) {
    t->start_ns = monotonic_ns();
}

f64 timer_elapsed_ms(const Timer* t) {
    return (f64)(monotonic_ns() - t->start_ns) / 1000000.0;
}

f64 timer_elapsed_us(const Timer* t) {
    return (f64)(monotonic_ns() - t->start_ns) / 1000.0;
}

f64 timer_elapsed_s(const Timer* t) {
    return (f64)(monotonic_ns() - t->start_ns) / 1000000000.0;
}

f64 timer_now_ms(void) {
    return (f64)monotonic_ns() / 1000000.0;
}

void timer_sleep_ms(u64 ms) {
    Sleep((DWORD)ms);
}

void timer_sleep_us(u64 us) {
    HANDLE timer = CreateWaitableTimer(nullptr, TRUE, nullptr);
    if (!timer) return;
    LARGE_INTEGER due;
    due.QuadPart = -(LONGLONG)(us * 10);
    SetWaitableTimer(timer, &due, 0, nullptr, nullptr, FALSE);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
#endif
