#pragma once

#include "core/container/ring_buffer/ring_buffer.hpp"
#include "core/container/string/string.hpp"
#include "core/define/types.h"
#include "core/io/console/ocstream.h"
#include "core/io/file/ofstream.h"
#include "logger_attrib.h"
#include "platform/thread.h"
#include <atomic>

class Logger {
public:
  static void submit(const LogMsg &logMsg);
  static void submit(LogMsg &&logMsg);

  static void flush();

  static void print(u32 id);
  static void print_all();

  // 增量读取日志（供 UI Console 使用）：回调 id >= fromId 的消息。
  using LogCallback = void (*)(const LogMsg &msg, void *userData);
  static void readMessages(u64 fromId, LogCallback cb, void *userData);
  static u32 latestId();

  static void write_all(const String &path); // Write all log file

  static SV_FORCE_INLINE u32 id() {
    return s_counter.fetch_add(1, std::memory_order_relaxed);
  }

private:
  static void printComplexMsg(const LogMsg &msg);
  static void printSimpleMsg(const LogMsg &msg);
  static void writeComplexMsg(const LogMsg &msg);
  static void writeSimpleMsg(const LogMsg &msg);

private:
  static Mutex s_mutex;

  static std::atomic<u32> s_counter;
  static RingBuffer<LogMsg> s_msgBuffer;

  static ConsoleOutputStream s_cout;
  static FileOutputStream s_fout;
};
