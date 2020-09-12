#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

#define MEMORY_SIZE 0x1000
#define MEMORY_PROGRAM_START 0x200
// Chip8's memory was shared between interpreter and program code.
// This interpreter doesn't follow this standard so the assigned memory size can be lower.
#define MEMORY_ARRAY_SIZE MEMORY_SIZE-MEMORY_PROGRAM_START
#define MEMORY_FONT_COUNT 16

typedef struct MEMORY {
    uint8_t general[MEMORY_ARRAY_SIZE];
} memory_t;

uint8_t memory_read_byte(memory_t *memory, uint16_t address);
void memory_write_byte(memory_t *memory, uint16_t address, uint8_t value);

void memory_clear(memory_t *memory);
void memory_load_rom_file(memory_t *memory, const char *path);
uint16_t memory_get_font_address(memory_t *memory, uint8_t character);

#endif //__MEMORY_H__
