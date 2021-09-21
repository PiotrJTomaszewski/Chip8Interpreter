#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <ncurses.h>

#include "cpu.h"
#include "memory.h"
#include "keyboard.h"

typedef struct DEBUG_WINDOW_STRUCT {
    WINDOW *window;
    int width;
    int height;
    int start_x;
    int start_y;
    const char *title;
} debug_window_t;

void debug_windows_init();
void debug_windows_end();

void debug_log(const char *msg);
void debug_logf(const char *format, ...);
void debug_print_memory(memory_t *memory);
void debug_print_stack(cpu_t *cpu);
void debug_print_registers(cpu_t *cpu);
void debug_print_pressed_keys(keyboard_pressed_t *pressed);
void debug_print_all(memory_t *memory, cpu_t *cpu, keyboard_pressed_t *pressed);

#endif //__DEBUG_H__
