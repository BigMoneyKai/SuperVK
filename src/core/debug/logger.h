#pragma once

#include "core/container/queue/queue.hpp"
#include "core/container/string/string.hpp"
#include "core/io/console/ocstream.h"
#include "core/io/file/ofstream.h"
#include "defines.h"
#include "logger_attrib.h"

class Logger {
public:
  static void submit(const LogMsg &logMsg);
  static void submit(LogMsg &&logMsg);

  static void flush();

  static void print(u32 id);
  static void print_all();

  static void write_all(const String &path); // Write all log file

  static SV_FORCE_INLINE u64 id() { return m_id++; }

private:
  static void printComplexMsg(const LogMsg &msg);
  static void printSimpleMsg(const LogMsg &msg);
  static void writeComplexMsg(const LogMsg &msg);
  static void writeSimpleMsg(const LogMsg &msg);

private:
  static u64 m_id;
  static Queue<LogMsg> m_msgQueue;

  static ConsoleOutputStream m_cout;
  static FileOutputStream m_fout;
};
