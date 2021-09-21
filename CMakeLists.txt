cmake_minimum_required(VERSION 3.10)
project(Chip8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

include(cmake/imgui.cmake)
add_subdirectory(interpreter)

configure_file("assets/sound_effect.wav" "${CMAKE_CURRENT_BINARY_DIR}/sound_effect.wav" COPYONLY)

include_directories(
    inc
    interpreter/inc
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(Chip8 ${SOURCES})
target_link_libraries(Chip8
    Chip8Interpreter
    ${IMGUI_LIBRARIES}
)
