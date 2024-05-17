cmake_minimum_required(VERSION 3.20)
project(InventoryTest VERSION 0.0.1)

file(GLOB_RECURSE UITEST_SOURCE src/*.cxx src/*.h)
message(STATUS Source: ${UITEST_SOURCE})

add_executable(InventoryTest ${UITEST_SOURCE}
        src/Sprite.cxx
        src/Sprite.h)

if (MSVC)
    target_compile_options(InventoryTest PRIVATE "/W4" "$<$<CONFIG:RELEASE>:/O2>")
else ()
    target_compile_options(InventoryTest PRIVATE "-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(InventoryTest PRIVATE "-stdlib=libc++")
    endif ()
endif ()

target_include_directories(InventoryTest PRIVATE src)

add_subdirectory(third_party/raylib SYSTEM)

target_link_libraries(InventoryTest raylib_static)
target_include_directories(InventoryTest SYSTEM PUBLIC third_party/raylib/src)

add_custom_command(
        TARGET InventoryTest
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/assets
)