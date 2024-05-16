cmake_minimum_required(VERSION 3.20)
project(UITest VERSION 0.0.1)

file(GLOB_RECURSE UITEST_SOURCE src/*.cxx src/*.h)

message(STATUS Source: ${UITEST_SOURCE})

add_executable(UITest ${UITEST_SOURCE})
target_include_directories(UITest PRIVATE src)

add_subdirectory(third_party/raylib SYSTEM)
add_dependencies(UITest raylib)
target_include_directories(UITest SYSTEM PUBLIC third_party/raylib/src)
