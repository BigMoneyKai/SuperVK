#pragma once

#include "defines.h"
#include "core/container/queue/queue.hpp"
#include "core/container/string/string.hpp"

enum class LogLevel : u8 {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Fatal,
};

enum class LogCatag : u8 {
    Container,
    Memory,
    Render,
    IO,
    Input,
    Scene,
    Vulkan,
    Asset,
    JobSystem,
    FileSystem,
    ECS,
    Unknown,

};

struct alignas(32) LogMsg {
    LogLevel level;
    LogCatag catag{LogCatag::Unknown};
    
    u32 line;
    u32 id;

    const char* msg;
    const char* file;
    const char* func;

    u64 timestamp;

};

class Logger {
public:
    static void submit(const LogMsg& logMsg);
    static void submit(LogMsg&& logMsg);
	template<class... Args>
	static void log(Args&&... args);

    static void flush();

    static void print(u32 id);
    static void print_all();
    static void write(const String& path, u32 id);
    static void write_all(const String& path);

private:
    static Queue<LogMsg> m_msgQueue;

};
