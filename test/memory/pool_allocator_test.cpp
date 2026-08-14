#include <gtest/gtest.h>

#include "core/memory/pool_allocator.h"

TEST(PoolAllocatorTest, AllocateAndDeallocate) {
    PoolAllocator pool(64, 128, 16); // 128 chunks of 64 bytes, 16-byte alignment

    void* a = pool.allocate(64);
    EXPECT_NE(a, nullptr);

    pool.deallocate(a);
}
