#pragma once

#include "core/define/types.h"

class Thread {
public:
  using EntryPoint = void (*)(void *);

  Thread() = default;
  ~Thread();

  Thread(const Thread &) = delete;
  Thread &operator=(const Thread &) = delete;

  Thread(Thread &&other) noexcept;
  Thread &operator=(Thread &&other) noexcept;

  void start(EntryPoint fn, void *userdata);
  void join();
  b32 joinable() const;

  static u32 hardware_concurrency();
  static void yield();

private:
  void *m_handle{nullptr};
};

class Mutex {
public:
  Mutex();
  ~Mutex();

  Mutex(const Mutex &) = delete;
  Mutex &operator=(const Mutex &) = delete;

  Mutex(Mutex &&) = delete;
  Mutex &operator=(Mutex &&) = delete;

  void lock();
  void unlock();
  b32 try_lock();

private:
  void *m_handle{nullptr};

  friend class CondVar;
};

class CondVar {
public:
  CondVar();
  ~CondVar();

  CondVar(const CondVar &) = delete;
  CondVar &operator=(const CondVar &) = delete;

  void wait(Mutex &mutex);
  void notify_one();
  void notify_all();

private:
  void *m_handle{nullptr};
};
