cmake_minimum_required(VERSION 3.10)
project(chip8interpreter)
include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SDL2_INCLUDE_DIR /usr/include/SDL2)
set(SDL2_LIBRARY /usr/lib64/libSDL2.so)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.c rom_selector.c display.c debug.c keyboard.c cpu.c memory.c timer_reg.c audio.c)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
target_link_libraries(${PROJECT_NAME} menu)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
