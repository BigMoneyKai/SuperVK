#include "memory.h"

#if defined(SV_PLATFORM_LINUX) || defined(SV_PLATFORM_APPLE)
#include <sys/mman.h>
#include <unistd.h>

void* platform_aligned_alloc(u64 size, u64 alignment) {
    void* ptr;
    if (posix_memalign(&ptr, alignment, size) != 0)
        return nullptr;
    return ptr;
}

void platform_aligned_free(void* ptr) {
    free(ptr);
}

void* virtual_reserve(u64 size) {
    void* ptr = mmap(
        nullptr,
        size,
        PROT_NONE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (ptr == MAP_FAILED)
        return nullptr;
    return ptr;
}

void virtual_release(void* ptr, u64 size) {
    munmap(ptr, size);
}

void virtual_commit(void* ptr, u64 size) {
    mprotect(ptr, size, PROT_READ | PROT_WRITE);
}

void virtual_decommit(void* ptr, u64 size) {
    mprotect(ptr, size, PROT_NONE);
    madvise(ptr, size, MADV_DONTNEED);
}

u64 page_size() {
    return static_cast<u64>(sysconf(_SC_PAGESIZE));
}

u64 allocation_granularity() {
    return page_size();
}

#elif defined(SV_PLATFORM_WINDOWS)
#include <Windows.h>
#include <malloc.h>

void* platform_aligned_alloc(u64 size, u64 alignment) {
    void* ptr = _aligned_malloc(size, alignment);
    return ptr;
}

void platform_aligned_free(void* ptr) {
    _aligned_free(ptr);
}

void* virtual_reserve(u64 size) {
    return VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
}

void virtual_release(void* ptr, u64 /*size*/) {
    VirtualFree(ptr, 0, MEM_RELEASE);
}

void virtual_commit(void* ptr, u64 size) {
    VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
}

void virtual_decommit(void* ptr, u64 size) {
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

u64 page_size() {
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return static_cast<u64>(info.dwPageSize);
}

u64 allocation_granularity() {
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return static_cast<u64>(info.dwAllocationGranularity);
}
#endif
