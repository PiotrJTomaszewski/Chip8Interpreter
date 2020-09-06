#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

#define MEMORY_SIZE 0x1000
#define MEMORY_FONT_NO 16

typedef struct MEMORY {
    uint8_t general[MEMORY_SIZE];
    uint16_t stack[16];
    uint16_t font_address[MEMORY_FONT_NO];
} memory_t;

memory_t *memory_init();
void memory_free(memory_t *memory);

void memory_load_rom_file(memory_t *memory, const char *path);
uint16_t memory_get_font_addr(memory_t *memory, uint8_t character);

#endif //__MEMORY_H__
