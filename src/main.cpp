#include <chrono>
#include <thread>
#include "system.h"
#include "interpreter_exception.h"
#include "gui.h"

#define __UNUSED(var) ((void)(var))

int main(int argc, char *argv[]) {
    __UNUSED(argc);
    __UNUSED(argv);

    System system;
    GUI gui(system);
    // system.load_rom_file("~/Documents/Chip8InterpreterLinux/roms/IBM Logo.ch8");
    while (!gui.get_should_close()) {
        auto start = std::chrono::high_resolution_clock::now();
        try {
            system.run_one_frame();
        } catch (const InterpreterException &e) {

        }
        gui.display();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = stop - start;
        if (duration < std::chrono::milliseconds(17)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(17) - duration);
        }
    }
}