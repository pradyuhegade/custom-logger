cmake_minimum_required(VERSION 3.14)
project(cpp_custom_logger VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include flat include/ directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(logger
    src/logger.cpp
)

target_include_directories(logger PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_executable(logger_example
    examples/main.cpp
)

target_link_libraries(logger_example PRIVATE logger)