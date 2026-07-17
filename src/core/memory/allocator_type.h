#pragma once

#include "defines.h"

enum class AllocatorType : u32 {
    Heap,
    Linear,
    Pool,
    Stack,

};