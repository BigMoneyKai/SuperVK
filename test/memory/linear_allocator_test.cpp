#include <gtest/gtest.h>

#include "core/memory/linear_allocator.h"

TEST(LinearAllocatorTest, AllocateAndReset) {
    LinearAllocator linear(1024);

    void* a = linear.allocate(128);
    EXPECT_NE(a, nullptr);

    linear.reset();

    void* b = linear.allocate(256);
    EXPECT_NE(b, nullptr);
    EXPECT_EQ(a, b); // after reset, bump pointer returns to start
}
