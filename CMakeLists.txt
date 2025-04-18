cmake_minimum_required(VERSION 3.14)
project(cpp_custom_logger VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Logger library
add_library(logger src/logger.cpp)

# Expose include/ during build & install
target_include_directories(logger PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Create an alias for usage like logger::logger
add_library(logger::logger ALIAS logger)

# Optional: Example app
add_executable(logger_example examples/main.cpp)
target_link_libraries(logger_example PRIVATE logger)

# Installation rules
include(GNUInstallDirs)

install(TARGETS logger
    EXPORT loggerTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets file
install(EXPORT loggerTargets
    FILE loggerTargets.cmake
    NAMESPACE logger::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logger
)

# Create the loggerConfig file for find_package()
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/loggerConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/loggerConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/loggerConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logger
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/loggerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/loggerConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logger
)