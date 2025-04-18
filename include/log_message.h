#ifndef LOG_MESSAGE_H
#define LOG_MESSAGE_H

#include "log_level.h"
#include "log_code.h"
#include <vector>
#include <chrono>
#include <thread>

struct LogMessage {
    uint64_t timestamp;              // epoch time in ms
    std::thread::id threadId;        // optional: can serialize as needed
    LogLevel level;
    LogCode code;
    std::vector<int64_t> args;       // additional arguments

    LogMessage(LogLevel lvl, LogCode c, std::vector<int64_t> a)
        : timestamp(currentTimeMillis()), threadId(std::this_thread::get_id()), level(lvl), code(c), args(std::move(a)) {}

private:
    static uint64_t currentTimeMillis() {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        return static_cast<uint64_t>(ms.count());
    }
};

#endif // LOG_MESSAGE_H