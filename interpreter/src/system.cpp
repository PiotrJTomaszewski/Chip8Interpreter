#include "system.h"

System::System() {
    memory = new Memory();
    io = new IO();
    cpu = new CPU(*memory, *io);
}

System::~System() {
    delete cpu;
    delete io;
    delete memory;
}

void System::load_rom_file(std::string path) {
    memory->load_rom_file(path);
}

void System::run_one_frame() {
    CPU::opcode_t opcode;
    for (int i=0; i<OPERATIONS_PER_FRAME; ++i) {
        opcode = cpu->fetch_next_instr();
        cpu->execute_instr(opcode);
    }
    io->timers.tick();
}
