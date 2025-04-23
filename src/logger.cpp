#include "logger.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <filesystem>

namespace fs = std::filesystem;

// Singleton instance
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Constructor
Logger::Logger() : running_(false) {}

// Destructor
Logger::~Logger() {
    stop();
}

// Start the logger
void Logger::start(const std::string& filename) {
    if (running_) return;

    // Ensure parent directory exists
    fs::path logPath = filename;
    if (!fs::exists(logPath.parent_path())) {
        fs::create_directories(logPath.parent_path());
    }

    fs::path parent = logPath.parent_path();
    if (!parent.empty() && !fs::exists(parent)) {
        fs::create_directories(parent);  // Only create if path is valid
    }

    logFile_.open(filename, std::ios::out | std::ios::app);
    if (!logFile_.is_open()) {
        std::cerr << "Logger failed to open file: " << filename << std::endl;
        return;
    }

    running_ = true;
    workerThread_ = std::thread(&Logger::workerThreadFunc, this);
}

// Stop the logger
void Logger::stop() {
    if (!running_) return;

    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        running_ = false;
    }

    cv_.notify_all();
    if (workerThread_.joinable()) {
        workerThread_.join();
    }

    if (logFile_.is_open()) {
        logFile_.close();
    }
}

// Log from any thread
void Logger::log(LogLevel level, LogCode code, const std::vector<int64_t>& args) {
    LogMessage msg(level, code, args);

    {
        std::cout << "[DEBUG] Logging: " << static_cast<int>(code) << "\n"; // DEBUG LINE
        std::lock_guard<std::mutex> lock(queueMutex_);
        logQueue_.push(std::move(msg));
    }

    cv_.notify_one();
}

// Worker thread function
void Logger::workerThreadFunc() {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        cv_.wait(lock, [this] { return !logQueue_.empty() || !running_; });

        while (!logQueue_.empty()) {
            LogMessage msg = std::move(logQueue_.front());
            logQueue_.pop();

            lock.unlock(); // Unlock while writing to file

            // Format and write to file
            logFile_ << msg.timestamp << " "
                     << static_cast<int>(msg.level) << " "
                     << static_cast<int>(msg.code);

            for (const auto& arg : msg.args) {
                logFile_ << " " << arg;
            }

            logFile_ << "\n";
            logFile_.flush(); // Optional

            lock.lock();
        }

        if (!running_ && logQueue_.empty()) break;
    }
}