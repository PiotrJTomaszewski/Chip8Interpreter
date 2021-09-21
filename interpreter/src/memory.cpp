#include <cstring>
#include <fstream>
#include "memory.h"

Memory::Memory() {

}

Memory::~Memory() {

}

uint8_t Memory::read(uint16_t address) {
    return data[address];
}

void Memory::write(uint16_t address, uint8_t value) {
    data[address] = value;
}

void Memory::clear(bool clear_font_data) {
    if (clear_font_data) {
        memset(data, 0, MEM_SIZE);
    } else {
        int font_bytes_count = FONT_SYMBOL_COUNT * FONT_HEIGHT;
        memset(data + font_bytes_count, 0, MEM_SIZE - font_bytes_count);
    }
}

void Memory::load_rom_file(std::string path) {
    clear(false);
    // TODO: Check if ROM can fit into memory
    int file_size;
    std::ifstream rom(path, std::ios::binary);
    file_size = static_cast<int>(rom.tellg());
    rom.seekg(0, std::ios::end);
    file_size = static_cast<int>(rom.tellg()) - file_size;

    // Skip to the program start
    rom.seekg(MEM_PROGRAM_START, std::ios::beg);
    rom.read(reinterpret_cast<char *>(data), file_size - MEM_PROGRAM_START);
    rom.close();
}

uint16_t Memory::get_font_char_address(uint8_t character) {
    return character * FONT_HEIGHT;
}
