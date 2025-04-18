#include "logger.h"
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>

void simulate_task(int thread_id) {
    for (int i = 0; i < 5; ++i) {
        Logger::getInstance().log(
            LogLevel::INFO,
            LogCode::TASK_COMPLETED,
            {thread_id, i}
        );

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    // Start the logger
    Logger::getInstance().start("D:/CODE/custom-logger/logs/app_encoded.log");

    // Launch multiple threads
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(simulate_task, i);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    // Stop the logger
    Logger::getInstance().stop();

    std::cout << "Logging completed. Encoded log written to logs/app_encoded.log\n";

    return 0;
}