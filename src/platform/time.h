#pragma once

#include "core/define/types.h"

namespace Platform {
struct Timer {
  u64 start_ns;
};

void timer_init(Timer *t);
void timer_start(Timer *t);
void timer_reset(Timer *t);
f64 timer_elapsed_ms(const Timer *t);
f64 timer_elapsed_us(const Timer *t);
f64 timer_elapsed_s(const Timer *t);

f64 timer_now_ms(void);
void timer_sleep_ms(u64 ms);
void timer_sleep_us(u64 us);
} // namespace Platform

using Timer = Platform::Timer;
using Platform::timer_init;
using Platform::timer_start;
using Platform::timer_reset;
using Platform::timer_elapsed_ms;
using Platform::timer_elapsed_us;
using Platform::timer_elapsed_s;
using Platform::timer_now_ms;
using Platform::timer_sleep_ms;
using Platform::timer_sleep_us;
