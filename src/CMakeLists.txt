cmake_minimum_required(VERSION 3.10)
project(chip8interpreter)
include_directories(${PROJECT_SOURCE_DIR})

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(chip8 main.c interpreter.c memory.c cpu.c display.c debug.c gui.c keyboard.c)
target_link_libraries(chip8 ${CURSES_LIBRARIES})