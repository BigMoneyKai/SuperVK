#pragma once

#include "defines.h"

enum class MemoryException : u8 {
    // Allocation
    MemoryLeak,                 // No freeing after allocation
    OutOfMemory,                // Out of memory
    HeapExhaustion,             // Out of heap space
    StackOverflow,              // Out of stack space
    AllocationTooLarge,         // Allocated too large size
    IntegerOverflow,            // Size calculation overflowed
    InvalidAlignment,           // Invalid alignment, not with power of two, not among {8, 16, 32, 64, 256, ...}

    // Corruption
    BufferOverflow,             // Write behind allocator buffer
    BufferUnderflow,            // Write ahead of allocator buffer
    HeaderCorruption,           // Header covered or corrupted
    AllocatorCorruption,        // Free list ruined or corrupted
    MemoryCorruption,           // Random address miswrotten

    // Lifetime
    UseAfterFree,               // Use freed memory
    UseUninitializedPointer,    // Use uninitialized pointer
    ReadUninitializedMemory,    // Read uninitialized memory chunk
    DanglingPointer,            // Pointing to a freed object
    WildPointer,                // Random pointer address

    // Pointer
    NullptrDereference,         // Dereference a nullptr
    MisalignedPointer,          // Misaligned pointer
    PointerArithmeticOverflow,  // Pointer arithmetic overflowed address space

    // Free
    DoubleFree,                 // Refreeing after freeing
    InvalidFree,                // Freeing memory not allocated by a certain allocator
    WrongAllocator,             // Allocated by allocator A, then freed by allocator B
    SizeMismatch,               // Freed size mismatched the allocated size
    InteriorPointerFree,        // Free called on an address inside an allocation

    // Virtual memory
    GuardPageViolation,         // Guard page accessed

    // Other
    InvalidReallocate,          // Invalid reallocation
    UnknownMemoryExcepton,

};

