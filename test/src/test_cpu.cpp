#include "doctest/doctest.h"
#include "system.h"
#include "cpu.h"

TEST_SUITE("CPU Tests") {
    TEST_CASE("ADD Vx, byte") {
        System system;
        auto &cpu = system.get_cpu();
        CPU::opcode_t opcode;

        SUBCASE("ADD Vx, byte - overflow") {
            // LD V1, 0xA7
            opcode.msb = 0x61;
            opcode.lsb = 0xA7;
            cpu.execute_instr(opcode);
            REQUIRE(cpu.get_general_reg(0x1) == 0xA7);

            // ADD V1, 0xB1
            opcode.msb = 0x71;
            opcode.lsb = 0xB1;
            cpu.execute_instr(opcode);
            CHECK(cpu.get_general_reg(0x1) == 0x58);
        }
    }

    TEST_CASE("ADD Vx, Vy") {
        System system;
        auto &cpu = system.get_cpu();
        CPU::opcode_t opcode;

        SUBCASE("ADD Vx, Vy - carry == 0") {
            // LD V2, 0xA7
            opcode.msb = 0x62;
            opcode.lsb = 0xA7;
            cpu.execute_instr(opcode);
            REQUIRE(cpu.get_general_reg(0x2) == 0xA7);

            // LD V3, 0x58
            opcode.msb = 0x63;
            opcode.lsb = 0x58;
            cpu.execute_instr(opcode);
            REQUIRE(cpu.get_general_reg(0x3) == 0x58);

            // ADD V2, V3
            opcode.msb = 0x82;
            opcode.lsb = 0x34;
            cpu.execute_instr(opcode);
            CHECK(cpu.get_general_reg(0x2) == 0xFF);
            CHECK(cpu.get_general_reg(0xF) == 0);
        }

        SUBCASE("ADD Vx, Vy - carry == 1") {
            // LD V2, 0xA7
            opcode.msb = 0x62;
            opcode.lsb = 0xA7;
            cpu.execute_instr(opcode);
            REQUIRE(cpu.get_general_reg(0x2) == 0xA7);

            // LD V3, 0x10
            opcode.msb = 0x63;
            opcode.lsb = 0x59;
            cpu.execute_instr(opcode);
            REQUIRE(cpu.get_general_reg(0x3) == 0x59);

            // ADD V2, V3
            opcode.msb = 0x82;
            opcode.lsb = 0x34;
            cpu.execute_instr(opcode);
            CHECK(cpu.get_general_reg(0x2) == 0x00);
            CHECK(cpu.get_general_reg(0xF) == 0x01);
        }
    }
}
