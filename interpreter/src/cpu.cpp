#include <cstdlib>
#include <ctime>
#include "cpu.h"
#include "interpreter_exception.h"

CPU::CPU(Memory &memory, IO &io): memory(memory), io(io) {
    srand(time(NULL));
    restart();
}

CPU::~CPU() {

}

void CPU::restart() {
    pc = 0x200;
    sp = 0;
}

inline void CPU::stack_push(uint16_t value) {stack[++sp] = value;}
inline uint16_t CPU::stack_pop() {return stack[sp--];}
inline uint8_t CPU::higher_nibble(uint8_t value) {return (value & 0xF0) >> 4;}
inline uint8_t CPU::lower_nibble(uint8_t value) {return value & 0x0F;}

// Get 12 lower bits of the opcode
inline uint16_t CPU::address_from_opcode(opcode_t opcode) {return ((opcode.msb & 0x0F) << 8) + opcode.lsb;}

inline uint8_t CPU::add_8bit(uint8_t a, uint8_t b) {return (a + b) % 256;}
inline uint16_t CPU::add_16bit(uint16_t a, uint16_t b) {return (a + b) % 65536;}

inline void CPU::skip_op() {pc = add_16bit(pc, 2);}


CPU::opcode_t CPU::fetch_next_instr() {
    opcode_t op;
    op.msb = memory.read(pc++);
    op.lsb = memory.read(pc++);
    return op;
}

void CPU::execute_instr(opcode_t opcode) {
    int x_reg_id = lower_nibble(opcode.msb);
    int y_reg_id = higher_nibble(opcode.lsb);
    switch (higher_nibble(opcode.msb)) {
        case 0x0:
            if (opcode.lsb == 0xE0) { // CLS
                io.display.clear();
            } else if (opcode.lsb == 0xEE) { // RET
                pc = stack_pop();
            } else {
                throw InterpreterException("Invalid operation 0x%x%x", opcode.msb, opcode.lsb);
            }
            break;
        case 0x1: // JP addr
            pc = address_from_opcode(opcode);
            break;
        case 0x2: // CALL addr
            stack_push(pc);
            pc = address_from_opcode(opcode);
            break;
        case 0x3: // SE Vx, byte
            if (general_reg[x_reg_id] == opcode.lsb) {
                skip_op();
            }
            break;
        case 0x4: // SNE Vx, byte
            if (general_reg[x_reg_id] != opcode.lsb) {
                skip_op();
            }
            break;
        case 0x5: // SE Vx, Vy
            if (general_reg[x_reg_id] == general_reg[y_reg_id]) {
                skip_op();
            }
            break;
        case 0x6: // LD Vx, byte
            general_reg[x_reg_id] = opcode.lsb;
            break;
        case 0x7: // ADD Vx, byte
            general_reg[x_reg_id] = add_8bit(general_reg[x_reg_id], opcode.lsb);
            break;
        case 0x8:
            switch(lower_nibble(opcode.lsb)) {
                case 0: // LD Vx, Vy
                    general_reg[x_reg_id] = general_reg[y_reg_id];
                    break;
                case 1: // OR Vx, Vy
                    general_reg[x_reg_id] = general_reg[x_reg_id] | general_reg[y_reg_id];
                    break;
                case 2: // AND Vx, Vy
                    general_reg[x_reg_id] = general_reg[x_reg_id] & general_reg[y_reg_id];
                    break;
                case 3: // XOR Vx, Vy
                    general_reg[x_reg_id] = general_reg[x_reg_id] ^ general_reg[y_reg_id];
                    break;
                case 4: // ADD Vx, Vy
                    {
                        int result = general_reg[x_reg_id] + general_reg[y_reg_id];
                        general_reg[CPU_FLAG_REG_ID] = result > 255 ? 1 : 0;
                        general_reg[x_reg_id] = result & 0xFF;
                    }
                    break;
                case 5: // SUB Vx, Vy
                    // TODO: Test this
                    general_reg[CPU_FLAG_REG_ID] = general_reg[x_reg_id] > general_reg[y_reg_id] ? 1 : 0;
                    general_reg[x_reg_id] = (general_reg[x_reg_id] - general_reg[y_reg_id]) & 0xFF;
                    break;
                case 6: // SHR Vx {, Vy}
                    general_reg[CPU_FLAG_REG_ID] = general_reg[x_reg_id] & 0x01;
                    general_reg[x_reg_id] >>= 1;
                    break;
                case 7: // SUBN Vx, Vy
                    // TODO: Test this
                    general_reg[CPU_FLAG_REG_ID] = general_reg[y_reg_id] > general_reg[x_reg_id] ? 1 : 0;
                    general_reg[x_reg_id] = (general_reg[y_reg_id] - general_reg[x_reg_id]) & 0xFF;
                    break;
                default:
                    if (lower_nibble(opcode.lsb) == 0x0E) { // SHL Vx {, Vy}
                        general_reg[CPU_FLAG_REG_ID] = general_reg[x_reg_id] & 0x80;
                        general_reg[x_reg_id] <<= 1;
                    } else {
                        throw InterpreterException("Invalid operation 0x%x%x", opcode.msb, opcode.lsb);
                    }
            }
            break;
        case 0x9: // SNE Vx, Vy
            if (general_reg[x_reg_id] != general_reg[y_reg_id]) {
                skip_op();
            }
            break;
        case 0xA: // LD I, addr
            mem_addr_reg = address_from_opcode(opcode);
            break;
        case 0xB: // JP V0, addr
            pc = add_16bit(address_from_opcode(opcode), static_cast<uint16_t>(general_reg[0]));
            break;
        case 0xC: // RND Vx, byte
            general_reg[x_reg_id] = (rand() % 256) & opcode.lsb;
            break;
        case 0xD: // DRW Vx, Vy, nibble
            {
                int x_pos = general_reg[x_reg_id];
                int y_pos = general_reg[y_reg_id];
                uint8_t byte;
                int bytes_to_read = lower_nibble(opcode.lsb);
                int collision = 0;
                for (int i=0; i<bytes_to_read; ++i) {
                    byte = memory.read(add_16bit(mem_addr_reg, i));
                    for (int bit_pos=8; bit_pos>=0; --bit_pos) {
                        if ((byte & (1 << bit_pos)) != 0) {
                            collision |= io.display.xor_pixel(y_pos+i, x_pos-bit_pos+7);
                        }
                    }
                }
                general_reg[CPU_FLAG_REG_ID] = collision;
            }
            break;
        case 0xE:
            if (opcode.lsb == 0x9E) { // SKP Vx
                if (io.keypad.get_is_pressed(general_reg[x_reg_id])) {
                    skip_op();
                }
            } else if (opcode.lsb == 0xA1) { // SKNP Vx
                if (!io.keypad.get_is_pressed(general_reg[x_reg_id])) {
                    skip_op();
                }
            } else {
                throw InterpreterException("Invalid operation 0x%x%x", opcode.msb, opcode.lsb);
            }
            break;
        case 0xF:
            switch(opcode.lsb) {
                case 0x07: // LD Vx, DelayTimer
                    general_reg[x_reg_id] = io.timers.get_delay_timer();
                    break;
                case 0x0A: // LD Vx, KEY (wait for key press)
                    io.keypad.wait_for_press();
                    general_reg[x_reg_id] = io.keypad.get_pressed_id();
                    break;
                case 0x15: // LD DelayTimer, Vx
                    io.timers.set_delay_timer(general_reg[x_reg_id]);
                    break;
                case 0x18: // LD SoundTimer, Vx
                    io.timers.set_sound_timer(general_reg[x_reg_id]);
                    break;
                case 0x1E: // ADD I, Vx
                    mem_addr_reg = add_16bit(mem_addr_reg, static_cast<uint16_t>(general_reg[x_reg_id]));
                    break;
                case 0x29: // LD Font, Vx
                    mem_addr_reg = memory.get_font_char_address(general_reg[x_reg_id]);
                    break;
                case 0x33: // LD BCD, Vx (Store BCD representation of Vx at I, I+1, I+2)
                    {
                        uint8_t value_to_store = general_reg[x_reg_id];
                        memory.write(mem_addr_reg, value_to_store / 100);
                        memory.write(add_16bit(mem_addr_reg, 1), (value_to_store % 100) / 10);
                        memory.write( add_16bit(mem_addr_reg, 2), value_to_store % 10);
                    }
                    break;
                case 0x55: // LD [I], Vx (Store V0 to Vx in memory)
                    for (int i=0; i<=x_reg_id; ++i) {
                        memory.write(add_16bit(mem_addr_reg, i), general_reg[i]);
                    }
                    mem_addr_reg = add_16bit(mem_addr_reg, static_cast<uint16_t>(add_8bit(x_reg_id, 1)));
                    break;
                case 0x65: // LD Vx, [I] (Read registers V0 to Vx from memory)
                    for (int i=0; i<=x_reg_id; ++i) {
                        general_reg[i] = memory.read(add_16bit(mem_addr_reg, i));
                    }
                    mem_addr_reg = add_16bit(mem_addr_reg, static_cast<uint16_t>(add_8bit(x_reg_id, 1)));
                    break;
                default:
                    throw InterpreterException("Invalid operation 0x%x%x", opcode.msb, opcode.lsb);
                    break;
            }
            break;
        default:
            throw InterpreterException("Invalid operation 0x%x%x", opcode.msb, opcode.lsb);
    }
}
