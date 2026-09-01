#include "logger.h"
#include "core/container/string/string.hpp"
#include "core/debug/logger_attrib.h"
#include "log_tag.h"
#include "platform/file.h"

#include <atomic>
#include <chrono>
#include <filesystem>
#include <format>
#include <string>
#include <utility>

Mutex Logger::s_mutex;
std::atomic<u32> Logger::s_counter;
RingBuffer<LogMsg> Logger::s_msgBuffer;
ConsoleOutputStream Logger::s_cout;
FileOutputStream Logger::s_fout;

static String create_log_path() {
  auto now = std::chrono::system_clock::now();
  String log_path(std::format("log/{:%Y-%m-%d_%H-%M-%S}.txt", now).c_str());

  return log_path;
}

void Logger::printComplexMsg(const LogMsg &msg) {
  std::string line =
      std::format("{}{} {}\n in {} at {}:{}\n timestamp: {}\n",
                  logLevelTags[static_cast<u32>(msg.level)],
                  logCatagTags[static_cast<u32>(msg.catag)], msg.msg.c_str(),
                  msg.func ? msg.func : "?", msg.file ? msg.file : "?",
                  msg.line, msg.timestamp);
  s_cout.write(line.data(), line.size());
}

void Logger::printSimpleMsg(const LogMsg &msg) {
  std::string line =
      std::format("{}{} {}\n", logLevelTags[static_cast<u32>(msg.level)],
                  logCatagTags[static_cast<u32>(msg.catag)], msg.msg.c_str());
  s_cout.write(line.data(), line.size());
}

void Logger::writeComplexMsg(const LogMsg &msg) {
  std::string line =
      std::format("{}{} {}\n in {} at {}:{}\n timestamp: {}\n",
                  logLevelTags[static_cast<u32>(msg.level)],
                  logCatagTags[static_cast<u32>(msg.catag)], msg.msg.c_str(),
                  msg.func ? msg.func : "?", msg.file ? msg.file : "?",
                  msg.line, msg.timestamp);
  s_fout.write(line.data(), line.size());
}

void Logger::writeSimpleMsg(const LogMsg &msg) {
  std::string line =
      std::format("{}:{} {}\n", logLevelTags[static_cast<u32>(msg.level)],
                  logCatagTags[static_cast<u32>(msg.catag)], msg.msg.c_str());
  s_fout.write(line.data(), line.size());
}

void Logger::submit(const LogMsg &logMsg) {
  s_mutex.lock();
  {
    s_msgBuffer.push_back(logMsg);
  }
  s_mutex.unlock();
}

void Logger::submit(LogMsg &&logMsg) {
  s_mutex.lock();
  {
    s_msgBuffer.push_back(std::move(logMsg));
  }
  s_mutex.unlock();
}

void Logger::flush() {
  s_mutex.lock();
  s_msgBuffer.clear();
  s_mutex.unlock();
}

void Logger::print(u32 id) {
  s_mutex.lock();
  {
    LogMsg msg = s_msgBuffer[id];
    if (msg.level == LogLevel::debug || msg.level == LogLevel::info) {
      printSimpleMsg(msg);
    } else {
      printComplexMsg(msg);
    }
  }
  s_mutex.unlock();
}

void Logger::print_all() {
  s_mutex.lock();
  for (const auto &msg : s_msgBuffer) {
    if (msg.level == LogLevel::debug || msg.level == LogLevel::info) {
      printSimpleMsg(msg);
    } else {
      printComplexMsg(msg);
    }
  }
  s_mutex.unlock();
}

void Logger::readMessages(u64 fromId, LogCallback cb, void *userData) {
  if (!cb)
    return;
  s_mutex.lock();
  for (const auto &msg : s_msgBuffer) {
    if (msg.id >= fromId)
      cb(msg, userData);
  }
  s_mutex.unlock();
}

u32 Logger::latestId() {
  return s_counter.load(std::memory_order_relaxed);
}

void Logger::write_all(const String &path) {
  String target = path.empty() ? create_log_path() : path;

  // make sure the log directory exists (the error_code overload does not throw)
  std::filesystem::path fsPath(target.c_str());
  if (fsPath.has_parent_path()) {
    std::error_code ec;
    std::filesystem::create_directories(fsPath.parent_path(), ec);
  }

  FileHandle *file = s_fout.open(target.c_str(), FileMode::append);
  if (!file) {
    std::string err =
        std::format("[Error] failed to open log file: {}\n", target.c_str());
    s_cout.write(err.data(), err.size());
    return;
  }

  s_mutex.lock();
  for (const auto &msg : s_msgBuffer) {
    if (msg.level == LogLevel::debug || msg.level == LogLevel::info) {
      writeSimpleMsg(msg);
    } else {
      writeComplexMsg(msg);
    }
  }
  s_mutex.unlock();

  s_fout.close();
}
