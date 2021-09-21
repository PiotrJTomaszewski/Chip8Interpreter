cmake_minimum_required(VERSION 3.10)
project(Chip8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

include(cmake/imgui.cmake)
add_subdirectory(interpreter)

include_directories(
    interpreter/inc
    lib/imgui_club/imgui_memory_editor
    lib/ImGuiFileDialog
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    )

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(Chip8 ${SOURCES} lib/ImGuiFileDialog/ImGuiFileDialog.cpp)
target_link_libraries(Chip8
    Chip8Interpreter
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${IMGUI_LIBRARIES}
    dl
    )
