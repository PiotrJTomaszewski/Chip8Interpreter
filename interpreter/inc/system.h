#pragma once
#include <string>
#include "cpu.h"
#include "memory.h"
#include "io/io.h"

class System {
public:
    System();
    ~System();
    void load_rom_file(std::string path);
    void run_one_frame();
private:
    static const int OPERATIONS_PER_FRAME = 8;
    CPU *cpu;
    Memory *memory;
    IO *io;
};
