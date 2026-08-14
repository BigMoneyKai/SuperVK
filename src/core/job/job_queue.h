#pragma once

#include "core/container/ring_buffer/ring_buffer.hpp"
#include "core/define/types.h"
#include "job.h"
#include "platform/thread.h"

class JobQueue {
public:
  JobQueue() = default;
  ~JobQueue() = default;

  JobQueue(const JobQueue &) = delete;
  JobQueue &operator=(const JobQueue &) = delete;

  void push_back(const Job &job);
  void push_back(Job &&job);
  b32 pop_front(Job &job);
  b32 try_pop(Job &job);
  void destroy();

private:
  RingBuffer<Job> m_jobs;
  Mutex m_mutex;
  CondVar m_cv;
  b32 m_destroyed{false};
};
