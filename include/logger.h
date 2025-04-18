#ifndef LOGGER_H
#define LOGGER_H

#include "log_level.h"
#include "log_code.h"
#include "log_message.h"

#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <fstream>
#include <string>

class Logger {
public:
    // Get singleton instance
    static Logger& getInstance();

    // Disable copy/move
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Logging API
    void log(LogLevel level, LogCode code, const std::vector<int64_t>& args = {});

    // Configuration
    void start(const std::string& filename = "logs/logdata.log");
    void stop();

private:
    Logger();                     // private constructor
    ~Logger();                    // destructor stops thread

    void workerThreadFunc();     // background thread function

    // Members
    std::queue<LogMessage> logQueue_;
    std::mutex queueMutex_;
    std::condition_variable cv_;
    std::thread workerThread_;
    std::atomic<bool> running_;
    std::ofstream logFile_;
};

#endif // LOGGER_H