cmake_minimum_required(VERSION 3.0)
project(spdlog CXX)
enable_testing()

set(${PROJECT_NAME}_SRCS
  src/spdlog.cc
  src/logger.cc
  src/async_logger.cc
  src/async_log_helper.cc
  src/format.cc
  src/registry.cc
  src/pattern_formatter.cc
  src/line_logger.cc
  src/file_helper.cc
  src/daily.cc
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRCS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_LIBRARY)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif(BUILD_TESTING)

if (BUILD_BENCHMARK)
    add_subdirectory(bench)
endif(BUILD_BENCHMARK)
