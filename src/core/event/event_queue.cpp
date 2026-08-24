#include "core/event/event_queue.h"

template <u64 size>
EventQueue<size>::EventQueue() {
  m_queue.resize(size);
}

  void push(Event event);
  b32 pop(Event* pEvent);