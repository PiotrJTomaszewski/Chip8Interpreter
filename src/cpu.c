#include <stdlib.h>

#include "cpu.h"
#include "debug.h"

void cpu_init(cpu_t *cpu, display_t *display, memory_t *memory, keyboard_pressed_t *pressed_keys) {
    cpu->pc = 0x200;
    cpu->sp = 0;
    cpu->display = display;
    cpu->memory = memory;
    cpu->pressed_keys = pressed_keys;
}

static inline void stack_push_(cpu_t *cpu, uint16_t value) {cpu->stack[++cpu->sp] = value;}
static inline uint16_t stack_pop_(cpu_t *cpu) {return cpu->stack[cpu->sp--];}

static inline uint8_t higher_nibble_(uint8_t value) {return (value & 0xF0) >> 4;}
static inline uint8_t lower_nibble_(uint8_t value) {return value & 0x0F;}

// Get 12 lower bits of the opcode
static inline uint16_t address_from_opcode_(opcode_t *opcode) {return ((opcode->msb & 0x0F) << 8) + opcode->lsb;}

static inline uint8_t add_8bit_(uint8_t a, uint8_t b) {return (a + b) % 0xFF;}
static inline uint16_t add_16bit_(uint16_t a, uint16_t b) {return (a + b) % 0xFFFF;}

static inline void skip_op_(cpu_t *cpu) {cpu->pc = (cpu->pc + 2) % 0xFFFF;}

void cpu_fetch_next_opcode(cpu_t *cpu, opcode_t *opcode) {
    opcode->msb = memory_read_byte(cpu->memory, cpu->pc);
    cpu->pc = add_16bit_(cpu->pc, 1);
    opcode->lsb = memory_read_byte(cpu->memory, cpu->pc);
    cpu->pc = add_16bit_(cpu->pc, 1);
}

// Multiple opcodes start with 0xF so handle them in a separate function
static inline void interpeter_exec_op0xF_(cpu_t *cpu, opcode_t *opcode) {
    int x_reg_id = lower_nibble_(opcode->msb);
    switch(opcode->lsb) {
        case 0x07: // LD Vx, DelayTimer
            cpu->general_reg[x_reg_id] =  cpu->delay_timer_reg;
            break;
        case 0x0A: // LD Vx, KEY (wait for key press)
            keyboard_wait_for_press(cpu->pressed_keys);
            cpu->general_reg[x_reg_id] = keyboard_get_pressed(cpu->pressed_keys);
            break;
        case 0x15: // LD DelayTimer, Vx
            cpu->delay_timer_reg = cpu->general_reg[x_reg_id];
            break;
        case 0x18: // LD SoundTimer, Vx
            cpu->sound_timer_reg = cpu->general_reg[x_reg_id];
            break;
        case 0x1E: // ADD I, Vx
            cpu->mem_addr_reg = add_16bit_(cpu->mem_addr_reg, (uint16_t)cpu->general_reg[x_reg_id]);
            break;
        case 0x29: // LD Font, Vx
            cpu->mem_addr_reg = memory_get_font_address(cpu->memory, cpu->general_reg[x_reg_id]);
            break;
        case 0x33: // LD BCD, Vx (Store BCD representation of Vx at I, I+1, I+2)
            {
                uint8_t value_to_store = cpu->general_reg[x_reg_id];
                memory_write_byte(cpu->memory, cpu->mem_addr_reg, value_to_store / 100);
                memory_write_byte(cpu->memory, add_16bit_(cpu->mem_addr_reg, 1), (value_to_store % 100) / 10);
                memory_write_byte(cpu->memory, add_16bit_(cpu->mem_addr_reg, 2), value_to_store % 10);
            }
            break;
        case 0x55: // LD [I], Vx (Store V0 to Vx in memory)
            for (int i=0; i<=x_reg_id; ++i) {
                memory_write_byte(cpu->memory, add_16bit_(cpu->mem_addr_reg, i), cpu->general_reg[i]);
            }
            cpu->mem_addr_reg = add_16bit_(cpu->mem_addr_reg, (uint16_t)add_8bit_(x_reg_id, 1));
            break;
        case 0x65: // LD Vx, [I] (Read registers V0 to Vx from memory)
            for (int i=0; i<=x_reg_id; ++i) {
                cpu->general_reg[i] = memory_read_byte(cpu->memory, add_16bit_(cpu->mem_addr_reg, i));
            }
            cpu->mem_addr_reg = add_16bit_(cpu->mem_addr_reg, (uint16_t)add_8bit_(x_reg_id, 1));
            break;
        default: 
            debug_logf("Invalid operation 0x%x%x", opcode->msb, opcode->lsb);
            break;
    }
}

void cpu_exec_opcode(cpu_t *cpu, opcode_t *opcode) {
    int x_reg_id = lower_nibble_(opcode->msb);
    int y_reg_id = higher_nibble_(opcode->lsb);
    switch (higher_nibble_(opcode->msb)) {
    case 0x0:
        if (opcode->lsb == 0xE0) { // CLS
            display_clear(cpu->display);
        } else if (opcode->lsb == 0xEE) { // RET
            cpu->pc = stack_pop_(cpu);
        } else {
            debug_logf("Invalid operation 0x%x%x", opcode->msb, opcode->lsb);
        }
        break;
    case 0x1: // JP addr
        cpu->pc = address_from_opcode_(opcode);
        break;
    case 0x2: // CALL addr
        stack_push_(cpu, cpu->pc);
        cpu->pc = address_from_opcode_(opcode);
        break;
    case 0x3: // SE Vx, byte
        if (cpu->general_reg[x_reg_id] == opcode->lsb) {
            skip_op_(cpu);
        }
        break;
    case 0x4: // SNE Vx, byte
        if (cpu->general_reg[x_reg_id] != opcode->lsb) {
            skip_op_(cpu);
        }
        break;
    case 0x5: // SE Vx, Vy
        if (cpu->general_reg[x_reg_id] == cpu->general_reg[y_reg_id]) {
            skip_op_(cpu);
        }
        break;
    case 0x6: // LD Vx, byte
        cpu->general_reg[x_reg_id] = opcode->lsb;
        break;
    case 0x7: // ADD Vx, byte
        cpu->general_reg[x_reg_id] = add_8bit_(cpu->general_reg[x_reg_id], opcode->lsb);
        break;
    case 0x8:
        switch(lower_nibble_(opcode->lsb)) {
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
                    cpu->general_reg[CPU_FLAG_REG_ID] = result > 255 ? 1 : 0;
                    cpu->general_reg[x_reg_id] = result & 0xFF;
                }
                break;
            case 5: // SUB Vx, Vy
                // TODO: Test this
                cpu->general_reg[CPU_FLAG_REG_ID] = cpu->general_reg[x_reg_id] > cpu->general_reg[y_reg_id] ? 1 : 0;
                cpu->general_reg[x_reg_id] = (cpu->general_reg[x_reg_id] - cpu->general_reg[y_reg_id]) & 0xFF;
                break;
            case 6: // SHR Vx {, Vy}
                cpu->general_reg[CPU_FLAG_REG_ID] = cpu->general_reg[x_reg_id] & 0x01;
                cpu->general_reg[x_reg_id] >>= 1;
                break;
            case 7: // SUBN Vx, Vy
                // TODO: Test this
                cpu->general_reg[CPU_FLAG_REG_ID] = cpu->general_reg[y_reg_id] > cpu->general_reg[x_reg_id] ? 1 : 0;
                cpu->general_reg[x_reg_id] = (cpu->general_reg[y_reg_id] - cpu->general_reg[x_reg_id]) & 0xFF;
                break;
            default:
                if (lower_nibble_(opcode->lsb) == 0x0E) { // SHL Vx {, Vy}
                    cpu->general_reg[CPU_FLAG_REG_ID] = cpu->general_reg[x_reg_id] & 0x80;
                    cpu->general_reg[x_reg_id] <<= 1;
                } else {
                    debug_logf("Invalid operation 0x%x%x", opcode->msb, opcode->lsb);
                }
        }
        break;
    case 0x9: // SNE Vx, Vy
        if (cpu->general_reg[x_reg_id] != cpu->general_reg[y_reg_id]) {
            skip_op_(cpu);
        }
        break;
    case 0xA: // LD I, addr
        cpu->mem_addr_reg = address_from_opcode_(opcode);
        break;
    case 0xB: // JP V0, addr
        cpu->pc = add_16bit_(address_from_opcode_(opcode), (uint16_t)cpu->general_reg[0]);
        break;
    case 0xC: // RND Vx, byte
        cpu->general_reg[x_reg_id] = (rand() % 256) & opcode->lsb;
        break;
    case 0xD: // DRW Vx, Vy, nibble
        {
            int x_pos = cpu->general_reg[x_reg_id];
            int y_pos = cpu->general_reg[y_reg_id];
            display_pixel_state_t pixel_state;
            uint8_t byte;
            int bytes_to_read = lower_nibble_(opcode->lsb);
            int collision = false;
            for (int i=0; i<bytes_to_read; ++i) {
                byte = memory_read_byte(cpu->memory, add_16bit_(cpu->mem_addr_reg, i));
                for (int bit_pos=8; bit_pos>=0; --bit_pos) { // TODO: Rework this
                    pixel_state = (byte & (1 << bit_pos)) >> bit_pos;
                    collision |= display_draw(cpu->display, y_pos+i, x_pos-bit_pos+7, pixel_state);
                }
            }
            cpu->general_reg[CPU_FLAG_REG_ID] = collision ? 1 : 0;
        }
        break;
    case 0xE:
        if (opcode->lsb == 0x9E) { // SKP Vx
            if (keyboard_is_keypad_key_pressed(cpu->pressed_keys, cpu->general_reg[x_reg_id])) {
                skip_op_(cpu);
            }
        } else if (opcode->lsb == 0xA1) { // SKNP Vx
            if (!keyboard_is_keypad_key_pressed(cpu->pressed_keys, cpu->general_reg[x_reg_id])) {
                skip_op_(cpu);
            }
        } else {
            debug_logf("Invalid operation 0x%x%x", opcode->msb, opcode->lsb);
        }
        break;
    case 0xF:
        // Multiple opcodes start with 0xF so handle them in a separate function
        interpeter_exec_op0xF_(cpu, opcode);
        break;
    default:
        debug_logf("Invalid operation 0x%x%x", opcode->msb, opcode->lsb);
    }
}
