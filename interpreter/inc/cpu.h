#pragma once
#include <cstdint>
#include "memory.h"
#include "io/io.h"

class CPU {
public:
    struct opcode_t { // Opcodes are big-endian
        uint8_t msb;
        uint8_t lsb;
    };

public:
    CPU(Memory &memory, IO &io);
    ~CPU();
    void restart();
    opcode_t fetch_next_instr();
    void execute_instr(opcode_t opcode);
    int get_general_reg_count();
    uint8_t get_general_reg(int id);
    uint16_t get_mem_addr_reg();
    uint16_t get_pc();
    uint8_t get_sp();
    uint16_t *get_stack();
    int get_stack_size();

private:
    static const int CPU_FLAG_REG_ID = 0x0F;
    static const int CPU_GENERAL_REGISTERS_COUNT = 16;
    static const int CPU_STACK_SIZE = 16;
    uint8_t general_reg[CPU_GENERAL_REGISTERS_COUNT];
    uint16_t mem_addr_reg; // Register I
    uint16_t pc; // Program Counter
    uint8_t sp; // Stack Pointer
    uint16_t stack[CPU_STACK_SIZE];
    bool waiting_for_key_press;

    Memory &memory;
    IO &io;

private:
    inline void stack_push(uint16_t value);
    inline uint16_t stack_pop();
    inline uint8_t higher_nibble(uint8_t value);
    inline uint8_t lower_nibble(uint8_t value);

    // Get 12 lower bits of the opcode
    inline uint16_t address_from_opcode(opcode_t opcode);

    inline uint8_t add_8bit(uint8_t a, uint8_t b);
    inline uint16_t add_16bit(uint16_t a, uint16_t b);

    inline void skip_op();
};
