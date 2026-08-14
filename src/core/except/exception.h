#pragma once

#include "core/container/string/string.hpp"

#include <exception>
#include <source_location>

class EngineException : public std::exception {
public:
  EngineException(const String &msg, std::source_location location =
                                         std::source_location::current())
      : m_msg(msg), m_location(location) {}

  const char *what() const noexcept override { return m_msg.c_str(); }
  const std::source_location &location() const { return m_location; }

private:
  String m_msg;
  std::source_location m_location;
};
