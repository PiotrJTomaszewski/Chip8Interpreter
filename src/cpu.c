#include <stdlib.h>

#include "cpu.h"
#include "debug.h"

cpu_t *cpu_init() {
    cpu_t *cpu = malloc(sizeof(cpu_t));
    cpu->pc = 0x200;
    cpu->sp = 0;
    return cpu;
}
void cpu_free(cpu_t *cpu) {
    free(cpu);
}
