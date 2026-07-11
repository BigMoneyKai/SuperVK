#pragma once

static std::

class Logger {
public:
    void trace();
    void debug();
    void info();
    void warning();
    void error();
    void fatal();

    void flush();

private:
};
