#pragma once

#include "core/container/ring_buffer/ring_buffer.hpp"
#include "core/define/types.h"
#include "core/event/event.h"
#include <atomic>

template <u64 size = 16> class EventQueue {
public:
  explicit EventQueue();

  void push(Event event);
  b32 pop(Event* pEvent);

private:
  struct Slot {
    std::atomic<b32> read{SV_FALSE};
    Event event;
  };
  // Non-blocking event queue (atomic + ring buffer)
  RingBuffer<Slot> m_queue;
  std::atomic<u64> m_write{0};
  u64 m_read{0};
};