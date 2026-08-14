#include <gtest/gtest.h>

#include "core/memory/heap_allocator.h"

TEST(HeapAllocatorTest, Allocate) {
    HeapAllocator& heap = HeapAllocator::instance();
    void* ptr = heap.allocate(64);
    EXPECT_NE(ptr, nullptr);
    heap.deallocate(ptr);
}
