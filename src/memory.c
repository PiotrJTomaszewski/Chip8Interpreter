#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "debug.h"

#define FONT_HEIGHT_ 5

static const uint8_t Font_[MEMORY_FONT_COUNT * FONT_HEIGHT_] = {
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

uint8_t memory_read_byte(memory_t *memory, uint16_t address) {
    if (address < MEMORY_FONT_COUNT * FONT_HEIGHT_) {
        return Font_[address];
    } else if (address < MEMORY_PROGRAM_START) { // Unused area
        return 0;
    } else if (address < MEMORY_SIZE) {
        return memory->general[address - MEMORY_PROGRAM_START];
    } else {
        debug_logf("Warning, invalid memory address was read: 0x%X", address);
        return 0;
    }
}

void memory_write_byte(memory_t *memory, uint16_t address, uint8_t value) {
    if (address < MEMORY_SIZE) {
        memory->general[address - MEMORY_PROGRAM_START] = value;
    } else {
        debug_logf("Warning, invalid memory address was written to: 0x%X", address);
    }
}

void memory_clear(memory_t *memory) {
    memset(memory->general, 0, MEMORY_ARRAY_SIZE);
}

void memory_load_rom_file(memory_t *memory, const char *path) {
    memory_clear(memory);
    // TODO: Check if ROM can fit in memory
    FILE *rom = fopen(path, "rb");
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);
    fread(memory->general, 1, rom_size, rom);
    fclose(rom);
}

uint16_t memory_get_font_address(memory_t *memory, uint8_t character) {
    if (character < MEMORY_FONT_COUNT)
        return character * FONT_HEIGHT_;
    else {
        debug_logf("Warning, invalid character font was accessed: 0x%X", character);
        return 0;
    }
}
