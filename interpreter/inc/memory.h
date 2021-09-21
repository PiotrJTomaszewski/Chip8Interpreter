#pragma once
#include <cstdint>
#include <string>

class Memory {
public:
    Memory();
    ~Memory();
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);
    void clear(bool clear_font_data);
    void load_rom_file(std::string path);
    uint16_t get_font_char_address(uint8_t character);
    bool get_is_rom_loaded();
    uint8_t *get_memory_raw();
    int get_memory_size();

private:
    static const int MEM_SIZE = 0x1000;
    static const int MEM_PROGRAM_START = 0x200;

    static const int FONT_SYMBOL_COUNT = 16;
    static const int FONT_HEIGHT = 5;

    // Lowest addresses of memory contains font data
    uint8_t data[MEM_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    bool is_rom_loaded;
};