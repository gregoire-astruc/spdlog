cmake_minimum_required(VERSION 3.0)
set(BENCHMARKS async bench-mt bench)
foreach(BENCH IN LISTS BENCHMARKS)
    add_executable(spdlog-${BENCH} spdlog-${BENCH}.cpp)
    target_link_libraries(spdlog-${BENCH} spdlog pthread)
    target_include_directories(spdlog-${BENCH} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)
    set_property(TARGET spdlog-${BENCH} PROPERTY CXX_STANDARD 11)
endforeach()

