#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "debug.h"

#define PROGRAM_MEMORY_START 0x200

#define MEMORY_FONT_HEIGHT 5

const uint8_t FONT[MEMORY_FONT_NO][MEMORY_FONT_HEIGHT] = {
    {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
    {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
    {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
    {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
    {0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
    {0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
    {0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
    {0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
    {0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
    {0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
    {0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
    {0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
    {0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
    {0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
    {0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
    {0xF0, 0x80, 0xF0, 0x80, 0x80}  // F
};

memory_t *memory_init() {
    return malloc(MEMORY_SIZE * sizeof(memory_t));
}

void memory_free(memory_t *memory) {
    free(memory);
}

// TODO: Implement memory access abstract layer to avoid copying font data
static void _memory_load_font(memory_t *memory) {
    for (int i=0; i<MEMORY_FONT_NO; ++i) {
        memory->font_address[i] = MEMORY_FONT_HEIGHT * i;
        memcpy(&memory->general[MEMORY_FONT_HEIGHT*i], FONT[i], MEMORY_FONT_NO * (sizeof(uint8_t)));
    }
}

void memory_load_rom_file(memory_t *memory, const char *path) {
    // TODO: Check if ROM can fit in memory
    FILE *rom = fopen(path, "rb");
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);
    fread(memory->general + PROGRAM_MEMORY_START, 1, rom_size, rom);
    fclose(rom);
    _memory_load_font(memory);
}

uint16_t memory_get_font_addr(memory_t *memory, uint8_t character) {
    return memory->font_address[character];
}
