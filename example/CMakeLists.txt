cmake_minimum_required(VERSION 3.0)

add_executable(spdlog-example example.cpp)
target_link_libraries(spdlog-example spdlog pthread)
target_include_directories(spdlog-example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)
set_property(TARGET spdlog-example PROPERTY CXX_STANDARD 11)

