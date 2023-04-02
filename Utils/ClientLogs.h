#pragma once

#include <fstream>
#ifndef Log
#define Log(...) Logger::log(__VA_ARGS__)
#endif
class Logger {
public:
    static void log(std::string logMsg) {
    };
};