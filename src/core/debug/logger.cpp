#include "logger.h"
#include "core/container/string/string.hpp"
#include "core/debug/logger_attrib.h"
#include "defines.h"
#include "log_tag.h"

#include <chrono>
#include <utility>

u64 Logger::m_id;
Queue<LogMsg> Logger::m_msgQueue;
ConsoleOutputStream Logger::m_cout;
FileOutputStream Logger::m_fout;

static String create_log_path() {
  long long timestamp = 1753660800;
  auto tp = std::chrono::system_clock::from_time_t(timestamp);
  time_t t = std::chrono::system_clock::to_time_t(tp);
  const char *time_str = ctime(&t);
  String log_path = "log/";
  log_path = log_path + time_str + ".log";

  return log_path;
}

void Logger::printComplexMsg(const LogMsg &msg) {
  m_cout << logLevelTags[static_cast<u32>(msg.level)]
         << logCatagTags[static_cast<u32>(msg.catag)] << " " << msg.msg
         << "\n in " << msg.func << " at " << msg.file << msg.line << '\n'
         << " timestamp: " << msg.timestamp << '\n';
}

void Logger::printSimpleMsg(const LogMsg &msg) {
  m_cout << logLevelTags[static_cast<u32>(msg.level)]
         << logCatagTags[static_cast<u32>(msg.catag)] << " " << msg.msg << '\n';
}

void Logger::writeComplexMsg(const LogMsg &msg) {
  m_fout << logLevelTags[static_cast<u32>(msg.level)]
         << logCatagTags[static_cast<u32>(msg.catag)] << " " << msg.msg
         << "\n in " << msg.func << " at " << msg.file << msg.line << '\n'
         << " timestamp: " << msg.timestamp << '\n';
}

void Logger::writeSimpleMsg(const LogMsg &msg) {
  m_fout << logLevelTags[static_cast<u32>(msg.level)]
         << logCatagTags[static_cast<u32>(msg.catag)] << " " << msg.msg << '\n';
}

void Logger::submit(const LogMsg &logMsg) { m_msgQueue.push_back(logMsg); }

void Logger::submit(LogMsg &&logMsg) {
  m_msgQueue.push_back(std::move(logMsg));
}

void Logger::flush() { m_msgQueue.clear(); }

void Logger::print(u32 id) {
  LogMsg msg = m_msgQueue[id];
  if (msg.level == LogLevel::Debug || msg.level == LogLevel::Info) {
    printSimpleMsg(msg);
  } else {
    printComplexMsg(msg);
  }
}

void Logger::print_all() {
  for (const auto &msg : m_msgQueue) {
    if (msg.level == LogLevel::Debug || msg.level == LogLevel::Info) {
      printSimpleMsg(msg);
    } else {
      printComplexMsg(msg);
    }
  }
}

void Logger::write_all(const String &path) {
  m_fout.open(create_log_path().c_str(), FileMode::ReadWrite);
  for (const auto &msg : m_msgQueue) {
    if (msg.level == LogLevel::Debug || msg.level == LogLevel::Info) {
      writeSimpleMsg(msg);
    } else {
      writeComplexMsg(msg);
    }
  }
}
