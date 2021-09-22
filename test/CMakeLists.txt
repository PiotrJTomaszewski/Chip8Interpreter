project(Chip8Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE TEST_SOURCES "./src/*.cpp")

include_directories(
    inc
    lib/doctest
    ../inc
    ../interpreter/inc
)

add_executable(Chip8Test ${TEST_SOURCES})
target_link_libraries(Chip8Test Chip8Interpreter)
