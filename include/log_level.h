#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

#include <cstdint>

enum class LogLevel : uint8_t {
    TRACE   = 0,
    DEBUG   = 1,
    INFO    = 2,
    WARNING = 3,
    ERROR   = 4
};

#endif // LOG_LEVEL_H