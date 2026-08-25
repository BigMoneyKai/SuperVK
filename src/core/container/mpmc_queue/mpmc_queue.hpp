#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"
#include "core/define/platform.h"
#include "core/define/arch.h"

#include <atomic>
#include <utility>

// Vyukov bounded MPMC (multi-producer / multi-consumer) lock-free queue.
//
// - Fixed capacity; MUST be a power of two (enforced at compile time).
// - push() never blocks: returns SV_FALSE when the queue is full.
// - pop()  never blocks: returns SV_FALSE when the queue is empty.
// - No locks and no runtime allocation; safe for any number of producer
//   and consumer threads.
//
// Algorithm: Dmitry Vyukov's bounded MPMC queue with per-slot sequence
// counters and relaxed CAS on the enqueue/dequeue positions. The release
// store of the sequence counter publishes the payload write to consumers;
// the acquire load makes the payload visible before it is read.
template <typename T, u64 Capacity> class MpmcQueue {
  SV_STATIC_ASSERT(Capacity > 0, "MpmcQueue capacity must be > 0");
  SV_STATIC_ASSERT((Capacity & (Capacity - 1)) == 0,
                   "MpmcQueue capacity must be a power of two");

public:
  MpmcQueue() {
    for (u64 i = 0; i < Capacity; ++i)
      m_cells[i].sequence.store(i, std::memory_order_relaxed);
    m_enqueuePos.store(0, std::memory_order_relaxed);
    m_dequeuePos.store(0, std::memory_order_relaxed);
  }

  MpmcQueue(const MpmcQueue &) = delete;
  MpmcQueue &operator=(const MpmcQueue &) = delete;

  // Returns SV_FALSE when the queue is full (item is dropped, nothing blocks).
  b32 push(const T &value) { return pushImpl(value); }
  b32 push(T &&value) { return pushImpl(std::move(value)); }

  // Returns SV_FALSE when the queue is empty. The item is moved into `out`.
  b32 pop(T *out) { return popImpl(out); }

  // Approximate number of pending items; only meaningful from one thread.
  u64 size() const {
    const u64 enqueue = m_enqueuePos.load(std::memory_order_relaxed);
    const u64 dequeue = m_dequeuePos.load(std::memory_order_relaxed);
    return enqueue - dequeue;
  }

private:
  struct Cell {
    std::atomic<u64> sequence;
    T data;
  };

  // Apple Silicon uses 128-byte cache lines; x86-64 and most other
  // arm64 targets (e.g. Raspberry Pi, Snapdragon) use 64.
  static constexpr u64 kCacheLine =
#if defined (SV_PLATFORM_APPLE) || defined(SV_ARCH_ARM64)
  128;
#else
  64;
#endif

  alignas(kCacheLine) std::atomic<u64> m_enqueuePos{0};
  alignas(kCacheLine) std::atomic<u64> m_dequeuePos{0};
  alignas(kCacheLine) Cell m_cells[Capacity];

  template <typename U> b32 pushImpl(U &&value) {
    u64 pos = m_enqueuePos.load(std::memory_order_relaxed);
    for (;;) {
      const u64 slot = pos & (Capacity - 1);
      const u64 seq = m_cells[slot].sequence.load(std::memory_order_acquire);
      const i64 dif = static_cast<i64>(seq) - static_cast<i64>(pos);
      if (dif == 0) {
        if (m_enqueuePos.compare_exchange_weak(pos, pos + 1,
                                               std::memory_order_relaxed))
          break;
      } else if (dif < 0) {
        return SV_FALSE; // full
      } else {
        pos = m_enqueuePos.load(std::memory_order_relaxed);
      }
    }
    Cell &cell = m_cells[pos & (Capacity - 1)];
    cell.data = std::forward<U>(value);
    m_cells[pos & (Capacity - 1)].sequence.store(pos + 1,
                                                 std::memory_order_release);
    return SV_TRUE;
  }

  b32 popImpl(T *out) {
    u64 pos = m_dequeuePos.load(std::memory_order_relaxed);
    for (;;) {
      const u64 slot = pos & (Capacity - 1);
      const u64 seq = m_cells[slot].sequence.load(std::memory_order_acquire);
      const i64 dif = static_cast<i64>(seq) - static_cast<i64>(pos + 1);
      if (dif == 0) {
        if (m_dequeuePos.compare_exchange_weak(pos, pos + 1,
                                               std::memory_order_relaxed))
          break;
      } else if (dif < 0) {
        return SV_FALSE; // empty
      } else {
        pos = m_dequeuePos.load(std::memory_order_relaxed);
      }
    }
    Cell &cell = m_cells[pos & (Capacity - 1)];
    if (out)
      *out = std::move(cell.data);
    m_cells[pos & (Capacity - 1)].sequence.store(pos + Capacity,
                                                 std::memory_order_release);
    return SV_TRUE;
  }
};
