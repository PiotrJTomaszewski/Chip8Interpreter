#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "cpu.h"
#include "gui.h"

void debug_error_print(char *message);
void debug_error_printf(char *format, ...);
void debug_info_print(char *message);
void debug_info_printf(char *format, ...);

void print_registers(cpu_t *cpu);

#endif //__DEBUG_H__
