#include "logger.h"

#include <utility>
void Logger::submit(const LogMsg& logMsg) {
    m_msgQueue.push_back(logMsg);
}

void Logger::submit(LogMsg&& logMsg) {
    m_msgQueue.push_back(std::move(logMsg));
}

template<class... Args>
void Logger::log(Args&&... args) {
    m_msgQueue.emplace_back(std::forward<LogMsg>((args)...));
}

void Logger::flush() {
    m_msgQueue.clear();
}
