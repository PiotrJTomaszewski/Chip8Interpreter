#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "interpreter.h"
#include "cpu.h"
#include "memory.h"
#include "debug.h"

static inline uint8_t higher_nibble(uint8_t value) { return (value & 0xF0) >> 4; }

static inline uint8_t lower_nibble(uint8_t value) { return value & 0x0F; }

// Get 12 lower bits of the opcode
static inline uint16_t address_from_opcode(opcode_t *opcode) { return ((opcode->msb & 0x0F) << 8) + opcode->lsb; }

interpreter_t *interpreter_init() {
    srand(time(NULL));
    interpreter_t *interpreter = malloc(sizeof(interpreter_t));
    interpreter->cpu = cpu_init();
    interpreter->memory = memory_init();
    memory_load_rom_file(interpreter->memory, "/home/piotr/tmp/chip8-test-rom/test_opcode.ch8");
    return interpreter;
}

void interpreter_free(interpreter_t *interpreter) {
    cpu_free(interpreter->cpu);
    memory_free(interpreter->memory);
    free(interpreter);
}

void interpreter_run(interpreter_t *interpreter) {
    cpu_t *cpu = interpreter->cpu;
    memory_t *memory = interpreter->memory;
    opcode_t opcode;
    for (int i=0; i<100; i++) {
        debug_info_printf("AD 0x%x", memory->general[cpu->pc]);
        opcode.msb = memory->general[cpu->pc++];
        opcode.lsb = memory->general[cpu->pc++];
        interpreter_exec_op(interpreter, &opcode);
    }
}

void interpreter_exec_op(interpreter_t *interpreter, opcode_t *opcode) {
    debug_info_printf("Executing 0x%x%x", opcode->msb, opcode->lsb);
    cpu_t *cpu = interpreter->cpu;
    int x_reg_id = lower_nibble(opcode->msb);
    int y_reg_id = higher_nibble(opcode->lsb);
    switch (higher_nibble(opcode->msb)) {
    case 0x0:
        if (opcode->lsb == 0xE0) { // CLS
            debug_error_print("Operation 0x00E0 CLS not implemented");
        } else if (opcode->lsb == 0xEE) { // RET
            cpu->pc = interpreter->memory->stack[cpu->sp--];
        } else {
            debug_error_printf("Invalid operation 0x%x%x", opcode->msb, opcode->lsb);
        }
        break;
    case 0x1: // JP addr
        cpu->pc = address_from_opcode(opcode);
        break;
    case 0x2: // CALL addr
        interpreter->memory->stack[++(cpu->sp)] = cpu->pc;
        cpu->pc = address_from_opcode(opcode);
        break;
    case 0x3: // SE Vx, byte
        if (cpu->general_reg[x_reg_id] == opcode->lsb) {
            cpu->pc += 2;
        }
        break;
    case 0x4: // SNE Vx, byte
        if (cpu->general_reg[x_reg_id] != opcode->lsb) {
            cpu->pc += 2;
        }
        break;
    case 0x5: // SE Vx, Vy
        if (cpu->general_reg[x_reg_id] == cpu->general_reg[y_reg_id]) {
            cpu->pc += 2;
        }
        break;
    case 0x6: // LD Vx, byte
        cpu->general_reg[x_reg_id] = opcode->lsb;
        break;
    case 0x7: // ADD Vx, byte
        cpu->general_reg[x_reg_id] = cpu->general_reg[x_reg_id] + opcode->lsb;
        break;
    case 0x8:
        switch(lower_nibble(opcode->lsb)) {
            case 0: // LD Vx, Vy
                cpu->general_reg[x_reg_id] = cpu->general_reg[y_reg_id];
                break;
            case 1: // OR Vx, Vy
                cpu->general_reg[x_reg_id] = cpu->general_reg[x_reg_id] | cpu->general_reg[y_reg_id];
                break;
            case 2: // AND Vx, Vy
                cpu->general_reg[x_reg_id] = cpu->general_reg[x_reg_id] & cpu->general_reg[y_reg_id];
                break;
            case 3: // XOR Vx, Vy
                cpu->general_reg[x_reg_id] = cpu->general_reg[x_reg_id] ^ cpu->general_reg[y_reg_id];
                break;
            case 4: // ADD Vx, Vy
                {
                    int result = cpu->general_reg[x_reg_id] + cpu->general_reg[y_reg_id];
                    cpu->general_reg[FLAG_REG_ID] = result > 255 ? 1 : 0;
                    cpu->general_reg[x_reg_id] = result & 0xFF;
                }
                break;
            case 5: // SUB Vx, Vy
                // TODO: Test this
                cpu->general_reg[FLAG_REG_ID] = cpu->general_reg[x_reg_id] > cpu->general_reg[y_reg_id] ? 1 : 0;
                cpu->general_reg[x_reg_id] = (cpu->general_reg[x_reg_id] - cpu->general_reg[y_reg_id]) & 0xFF;
                break;
            case 6: // SHR Vx {, Vy}
                cpu->general_reg[FLAG_REG_ID] = cpu->general_reg[x_reg_id] & 0x01;
                cpu->general_reg[x_reg_id] >>= 1;
                break;
            case 7: // SUBN Vx, Vy
                // TODO: Test this
                cpu->general_reg[FLAG_REG_ID] = cpu->general_reg[y_reg_id] > cpu->general_reg[x_reg_id] ? 1 : 0;
                cpu->general_reg[x_reg_id] = (cpu->general_reg[y_reg_id] - cpu->general_reg[x_reg_id]) & 0xFF;
                break;
            default:
                if (lower_nibble(opcode->lsb) == 0x0E) { // SHL Vx {, Vy}
                    cpu->general_reg[FLAG_REG_ID] = cpu->general_reg[x_reg_id] & 0x80;
                    cpu->general_reg[x_reg_id] <<= 1;
                } else {
                    debug_error_printf("Invalid operation 0x%x%x", opcode->msb, opcode->lsb);
                }
        }
        break;
    case 0x9: // SNE Vx, Vy
        if (cpu->general_reg[x_reg_id] != cpu->general_reg[y_reg_id]) {
            cpu->pc += 2;
        }
        break;
    case 0xA: // LD I, addr
        cpu->mem_addr_reg = address_from_opcode(opcode);
        break;
    case 0xB: // JP V0, addr
        cpu->pc = address_from_opcode(opcode) + cpu->general_reg[0];
        break;
    case 0xC: // RND Vx, byte
        cpu->general_reg[x_reg_id] = (rand() % 256) & opcode->lsb;
        break;
    case 0xD: // DRW Vx, Vy, nibble
        debug_error_print("Opcode 0xDxyn DRW not implemented");
        break;
    case 0xE:
        if (opcode->lsb == 0x9E) { // SKP Vx
            debug_error_print("Operation 0xEx9E SKP Vx not implemented");
        } else if (opcode->lsb == 0xA1) {
            debug_error_print("Operation 0xExA1 SKNP Vx not implemented");
        } else {
            debug_error_printf("Invalid operation 0x%x%x", opcode->msb, opcode->lsb);
        }
        break;
    case 0xF:
        debug_error_print("Operation 0xFxxx not implemented");
        break;
    default:
        debug_error_printf("Invalid operation 0x%x%x", opcode->msb, opcode->lsb);
    }
}
