#ifndef LOG_CODE_H
#define LOG_CODE_H

#include <cstdint>

enum class LogCode : uint16_t {
    DB_CONNECT_SUCCESS = 100,
    DB_CONNECT_FAIL    = 101,
    USER_LOGIN         = 102,
    FILE_NOT_FOUND     = 103,
    TASK_COMPLETED     = 104,
    // Add more as needed
};

#endif // LOG_CODE_H