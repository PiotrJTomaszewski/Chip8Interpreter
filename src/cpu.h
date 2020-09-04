#ifndef __CPU_H__
#define __CPU_H__

#include <stdint.h>

#define FLAG_REG_ID 0x0F
#define GENERAL_REGISTERS_NO 16

typedef struct CPU {
    uint8_t general_reg[GENERAL_REGISTERS_NO];
    uint16_t mem_addr_reg; // Register I
    uint8_t delay_timer_reg;
    uint8_t sound_timer_reg;
    uint16_t pc; // Program Counter
    uint8_t sp; // Stack Pointer
} cpu_t;

typedef struct OPCODE { // Opcodes are big-endian
    uint8_t msb;
    uint8_t lsb;
} opcode_t;

cpu_t *cpu_init();
void cpu_free();

#endif //__CPU_H__
