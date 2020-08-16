#include <stdlib.h>
#include <stdio.h>

#include "memory.h"
#include "debug.h"

#define PROGRAM_MEMORY_START 0x200

memory_t *memory_init() {
    return malloc(MEMORY_SIZE * sizeof(memory_t));
}

void memory_free(memory_t *memory) {
    free(memory);
}

void memory_load_rom_file(memory_t *memory, const char *path) {
    // TODO: Check if ROM can fit in memory
    FILE *rom = fopen(path, "rb");
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);
    fread(memory->general + PROGRAM_MEMORY_START, 1, rom_size, rom);
    fclose(rom);
}
