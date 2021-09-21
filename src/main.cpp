#include <chrono>
#include <thread>
#include "system.h"
#include "interpreter_exception.h"
#include "gui.h"
#include "sound.h"

#define __UNUSED(var) ((void)(var))

int main(int argc, char *argv[]) {
    __UNUSED(argc);
    __UNUSED(argv);

    System system;
    GUI gui(system);

    while (!gui.get_should_close()) {
        auto start = std::chrono::high_resolution_clock::now();
        try {
            system.run_one_frame();
        } catch (const InterpreterException &e) {
            // TODO: Log
        }
        gui.display();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = stop - start;
        if (duration < std::chrono::milliseconds(17)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(17) - duration);
        }
    }
}
