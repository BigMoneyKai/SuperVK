#pragma once

#include "app/app.h"
#include "core/debug/logger.h"

#include <iostream>

#include "defines.h"

inline void safeExitSignal() { std::cout << "Exited safely" << std::endl; }

#if defined(SV_PLATFORM_WINDOWS)
#define MAIN_ENTRY()                                                           \
  int main(i32 argc, const char *argv[]) {                                     \
    App app;                                                                   \
    app.init();                                                                \
    app.run();                                                                 \
    app.destroy();                                                             \
    Logger::print_all();                                                       \
    safeExitSignal();                                                          \
  }
#elif defined(SV_PLATFORM_APPLE) || defined(SV_PLATFORM_LINUX)
#define MAIN_ENTRY()                                                           \
  int main(i32 argc, const char *argv[]) {                                     \
    App app;                                                                   \
    app.init();                                                                \
    app.run();                                                                 \
    app.destroy();                                                             \
    Logger::print_all();                                                       \
    safeExitSignal();                                                          \
  }
#else
#define MAIN_ENTRY()                                                           \
  int main(i32 argc, const char *argv[]) {                                     \
    FATAL(LogCatag::Platform,                                                  \
          "Unsupported platform, please choose Windows, Linux or MacOS");      \
  }
#endif
