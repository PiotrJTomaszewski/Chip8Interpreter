project(Chip8Interpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

include_directories(inc)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(Chip8Interpreter ${SOURCES})
