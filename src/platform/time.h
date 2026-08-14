#pragma once

#include "core/define/types.h"

typedef struct {
  u64 start_ns;
} Timer;

void timer_init(Timer *t);
void timer_start(Timer *t);
void timer_reset(Timer *t);
f64 timer_elapsed_ms(const Timer *t);
f64 timer_elapsed_us(const Timer *t);
f64 timer_elapsed_s(const Timer *t);

f64 timer_now_ms(void);
void timer_sleep_ms(u64 ms);
void timer_sleep_us(u64 us);
