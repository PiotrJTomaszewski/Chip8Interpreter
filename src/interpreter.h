#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <stdint.h>

#include "cpu.h"
#include "memory.h"
#include "display.h"

typedef struct INTERPRETER {
    cpu_t *cpu;
    memory_t *memory;
    display_t display;
} interpreter_t;

interpreter_t *interpreter_init(const char *rom_file_path);
void interpreter_free(interpreter_t *interpreter);

void interpreter_run(interpreter_t *interpreter);
void interpreter_exec_op(interpreter_t *interpreter, opcode_t *opcode);

#endif //__INTERPRETER_H__
