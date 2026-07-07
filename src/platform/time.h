#pragma once
#include "defines.h"

class Timer {
public:
    void   start();
    f64    elapsed_ms();
    f64    elapsed_us();
    static f64 now_ms();
};
