#ifndef __CPU_H__
#define __CPU_H__

#include <stdint.h>

#include "display.h"
#include "memory.h"
#include "keyboard.h"

#define CPU_FLAG_REG_ID 0x0F
#define CPU_GENERAL_REGISTERS_COUNT 16
#define CPU_STACK_SIZE 16

typedef struct CPU {
    uint8_t general_reg[CPU_GENERAL_REGISTERS_COUNT];
    uint16_t mem_addr_reg; // Register I
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t pc; // Program Counter
    uint8_t sp; // Stack Pointer
    uint16_t stack[CPU_STACK_SIZE];

    display_t *display;
    memory_t *memory;
    keyboard_pressed_t *pressed_keys;
} cpu_t;

typedef struct OPCODE { // Opcodes are big-endian
    uint8_t msb;
    uint8_t lsb;
} opcode_t;

void cpu_init(cpu_t *cpu, display_t *display, memory_t *memory, keyboard_pressed_t *pressed_keys);

void cpu_fetch_next_opcode(cpu_t *cpu, opcode_t *opcode);
void cpu_exec_opcode(cpu_t *cpu, opcode_t *opcode);

#endif //__CPU_H__
