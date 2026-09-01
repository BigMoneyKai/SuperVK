#include "thread.h"
#include "core/define/platform.h"

namespace Platform {

#if defined(SV_PLATFORM_LINUX) || defined(SV_PLATFORM_APPLE)
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

// ── Thread ──────────────────────────────────────────────────────────────────

struct ThreadStart {
  Thread::EntryPoint fn;
  void *userdata;
};

static void *thread_proc(void *arg) {
  auto *ts = static_cast<ThreadStart *>(arg);
  ts->fn(ts->userdata);
  delete ts;
  return nullptr;
}

Thread::Thread(Thread &&other) noexcept : m_handle(other.m_handle) {
  other.m_handle = nullptr;
}

Thread &Thread::operator=(Thread &&other) noexcept {
  if (this != &other) {
    if (m_handle)
      pthread_detach(*static_cast<pthread_t *>(m_handle));
    m_handle = other.m_handle;
    other.m_handle = nullptr;
  }
  return *this;
}

Thread::~Thread() {
  if (m_handle)
    pthread_detach(*static_cast<pthread_t *>(m_handle));
}

void Thread::start(EntryPoint fn, void *userdata) {
  auto *ts = new ThreadStart{fn, userdata};
  auto *pt = new pthread_t();
  pthread_create(pt, nullptr, thread_proc, ts);
  m_handle = pt;
}

void Thread::join() {
  if (!m_handle)
    return;
  pthread_join(*static_cast<pthread_t *>(m_handle), nullptr);
  delete static_cast<pthread_t *>(m_handle);
  m_handle = nullptr;
}

b32 Thread::joinable() const { return m_handle != nullptr; }

u32 Thread::hardware_concurrency() {
  long n = sysconf(_SC_NPROCESSORS_ONLN);
  return n > 0 ? static_cast<u32>(n) : 1;
}

void Thread::yield() { sched_yield(); }

// ── Mutex ───────────────────────────────────────────────────────────────────

Mutex::Mutex() {
  auto *pm = new pthread_mutex_t();
  pthread_mutex_init(pm, nullptr);
  m_handle = pm;
}

Mutex::~Mutex() {
  if (m_handle) {
    pthread_mutex_destroy(static_cast<pthread_mutex_t *>(m_handle));
    delete static_cast<pthread_mutex_t *>(m_handle);
  }
}

void Mutex::lock() {
  pthread_mutex_lock(static_cast<pthread_mutex_t *>(m_handle));
}

void Mutex::unlock() {
  pthread_mutex_unlock(static_cast<pthread_mutex_t *>(m_handle));
}

b32 Mutex::try_lock() {
  return pthread_mutex_trylock(static_cast<pthread_mutex_t *>(m_handle)) == 0;
}

// ── CondVar ─────────────────────────────────────────────────────────────────

CondVar::CondVar() {
  auto *pc = new pthread_cond_t();
  pthread_cond_init(pc, nullptr);
  m_handle = pc;
}

CondVar::~CondVar() {
  if (m_handle) {
    pthread_cond_destroy(static_cast<pthread_cond_t *>(m_handle));
    delete static_cast<pthread_cond_t *>(m_handle);
  }
}

void CondVar::wait(Mutex &mutex) {
  pthread_cond_wait(static_cast<pthread_cond_t *>(m_handle),
                    static_cast<pthread_mutex_t *>(mutex.m_handle));
}

void CondVar::notify_one() {
  pthread_cond_signal(static_cast<pthread_cond_t *>(m_handle));
}

void CondVar::notify_all() {
  pthread_cond_broadcast(static_cast<pthread_cond_t *>(m_handle));
}

// ── Windows ─────────────────────────────────────────────────────────────────

#elif defined(SV_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct ThreadStart {
  Thread::EntryPoint fn;
  void *userdata;
};

static DWORD WINAPI thread_proc(LPVOID arg) {
  auto *ts = static_cast<ThreadStart *>(arg);
  ts->fn(ts->userdata);
  delete ts;
  return 0;
}

Thread::Thread(Thread &&other) noexcept : m_handle(other.m_handle) {
  other.m_handle = nullptr;
}

Thread &Thread::operator=(Thread &&other) noexcept {
  if (this != &other) {
    if (m_handle)
      CloseHandle(static_cast<HANDLE>(m_handle));
    m_handle = other.m_handle;
    other.m_handle = nullptr;
  }
  return *this;
}

Thread::~Thread() {
  if (m_handle)
    CloseHandle(static_cast<HANDLE>(m_handle));
}

void Thread::start(EntryPoint fn, void *userdata) {
  auto *ts = new ThreadStart{fn, userdata};
  m_handle = CreateThread(nullptr, 0, thread_proc, ts, 0, nullptr);
}

void Thread::join() {
  if (!m_handle)
    return;
  WaitForSingleObject(static_cast<HANDLE>(m_handle), INFINITE);
  CloseHandle(static_cast<HANDLE>(m_handle));
  m_handle = nullptr;
}

b32 Thread::joinable() const { return m_handle != nullptr; }

u32 Thread::hardware_concurrency() {
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  return info.dwNumberOfProcessors;
}

void Thread::yield() { SwitchToThread(); }

Mutex::Mutex() {
  auto *cs = new CRITICAL_SECTION();
  InitializeCriticalSection(cs);
  m_handle = cs;
}

Mutex::~Mutex() {
  if (m_handle) {
    DeleteCriticalSection(static_cast<CRITICAL_SECTION *>(m_handle));
    delete static_cast<CRITICAL_SECTION *>(m_handle);
  }
}

void Mutex::lock() {
  EnterCriticalSection(static_cast<CRITICAL_SECTION *>(m_handle));
}

void Mutex::unlock() {
  LeaveCriticalSection(static_cast<CRITICAL_SECTION *>(m_handle));
}

b32 Mutex::try_lock() {
  return TryEnterCriticalSection(static_cast<CRITICAL_SECTION *>(m_handle)) !=
         0;
}

CondVar::CondVar() {
  auto *cv = new CONDITION_VARIABLE();
  InitializeConditionVariable(cv);
  m_handle = cv;
}

CondVar::~CondVar() {
  if (m_handle) {
    delete static_cast<CONDITION_VARIABLE *>(m_handle);
  }
}

void CondVar::wait(Mutex &mutex) {
  SleepConditionVariableCS(static_cast<CONDITION_VARIABLE *>(m_handle),
                           static_cast<CRITICAL_SECTION *>(mutex.m_handle),
                           INFINITE);
}

void CondVar::notify_one() {
  WakeConditionVariable(static_cast<CONDITION_VARIABLE *>(m_handle));
}

void CondVar::notify_all() {
  WakeAllConditionVariable(static_cast<CONDITION_VARIABLE *>(m_handle));
}
#endif

} // namespace Platform
