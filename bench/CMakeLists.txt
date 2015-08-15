cmake_minimum_required(VERSION 3.0)
set(BENCH_SRCS
    spdlog-async.cpp
)

add_executable(spdlog-bench ${BENCH_SRCS})
target_link_libraries(spdlog-bench spdlog pthread)
target_include_directories(spdlog-bench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)
set_property(TARGET spdlog-bench PROPERTY CXX_STANDARD 11)

