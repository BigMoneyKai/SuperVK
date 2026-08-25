#pragma once

#include "core/container/mpmc_queue/mpmc_queue.hpp"
#include "core/define/types.h"
#include "core/event/event.h"

// Thread-safe lock-free event queue.
//
// Any number of threads may push() at any time; the main loop drains the
// queue once per frame with pop(). Both operations never block:
//   - push() returns SV_FALSE when the queue is full (event dropped);
//   - pop()  returns SV_FALSE when the queue is empty.
// Capacity must be a power of two (static_assert in MpmcQueue).
template <u64 capacity = 256> class EventQueue {
public:
  b32 push(const Event &event) { return m_queue.push(event); }
  b32 push(Event &&event) { return m_queue.push(std::move(event)); }

  b32 pop(Event *pEvent) { return m_queue.pop(pEvent); }

  u64 size() const { return m_queue.size(); }

private:
  MpmcQueue<Event, capacity> m_queue;
};
