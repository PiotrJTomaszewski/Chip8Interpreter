#include <chrono>
#include <thread>
#include "system.h"

#define __UNUSED(var) ((void)(var))

int main(int argc, char *argv[]) {
    __UNUSED(argc);
    __UNUSED(argv);

    System system;
    system.load_rom_file("~/Documents/Chip8InterpreterLinux/roms/IBM Logo.ch8");
    bool should_close = false;
    while (!should_close) {
        auto start = std::chrono::high_resolution_clock::now();
        system.run_one_frame();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = stop - start;
        if (duration < std::chrono::milliseconds(17)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(17) - duration);
        }
    }
}