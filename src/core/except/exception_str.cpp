#include "exception_str.h"

const char* MemoryExceptionString[] = {
    // Allocation
    "memory leaked",
    "out of memory",
    "out of heap space",
    "out of stack space",
    "allocated too large space",
    "integer size calculation overflowed",
    "invalid alignment, any alignment number should be the power of 2, such as 8, 16, 32...",

    // Corruption
    "write behind allocator buffer",
    "write ahead of allocator buffer",
    "header covered or corrupted",
    "allocator free list ruined or destroyed",
    "random address miswrotten",

    // Lifetime
    "used freed memory",
    "used uninitialized pointer",
    "read uninitialized memory",
    "pointing to a freed object",
    "random pointer address",

    // Pointer
    "dereference a null pointer(nullptr)",
    "misaligned pointer",
    "pointer arithmetic overflowed address space",
    
    // Free
    "pointer double-freed",
    "freed memory not allocated by a certain allocator",
    "not freed by own allocator",
    "freed size mismatched the allocated size",
    "free called on an address inside an allocation",

    // Virtual memory
    "guard page accessed",

    // Other
    "Invalid reallocation",
    "Unknown memory exception/error",
};
