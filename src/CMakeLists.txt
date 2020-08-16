cmake_minimum_required(VERSION 2.4)
project(chip8interpreter)
include_directories(${PROJECT_SOURCE_DIR})
add_executable(chip8 main.c interpreter.c memory.c cpu.c debug.c)