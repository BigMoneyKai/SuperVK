#include <gtest/gtest.h>

#include "core/memory/stack_allocator.h"

TEST(StackAllocatorTest, AllocateAndReset) {
    StackAllocator stack(1024);

    void* a = stack.allocate(64);
    EXPECT_NE(a, nullptr);

    Marker marker{stack.used()};

    void* b = stack.allocate(128);
    EXPECT_NE(b, nullptr);

    stack.reset_to(marker);

    void* c = stack.allocate(128);
    EXPECT_EQ(b, c); // after reset_to, same memory reused
}
