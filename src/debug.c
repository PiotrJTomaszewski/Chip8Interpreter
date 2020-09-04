#include <stdio.h>
#include <stdarg.h>

#include "debug.h"

#define REGISTERS_SECOND_COLUMN 14

void debug_error_print(char *message) {
    // puts(message);
}

void debug_error_printf(char *format, ...) {
    va_list valist;
    va_start(valist, format);
    // vprintf(format, valist);
    va_end(valist);
    // puts("");
}

void debug_info_print(char *message) {
    // puts(message);
}

void debug_info_printf(char *format, ...) {
    va_list valist;
    va_start(valist, format);
    // vprintf(format, valist);
    va_end(valist);
    // puts("");
}

void print_registers(cpu_t *cpu) {
    char buf[30]; // TODO: Choose a smaller number
    gui_window_t *window = &gui_windows_global[GUI_WINDOW_REGISTERS];
    for (int i=0; i<GENERAL_REGISTERS_NO; ++i) {
        sprintf(buf, "%X: 0x%02X", i, cpu->general_reg[i]);
        gui_window_print(window, i, 0, buf);
    }
    sprintf(buf, "REGISTER I: 0x%04X", cpu->mem_addr_reg);
    gui_window_print(window, 0, REGISTERS_SECOND_COLUMN, buf);
    sprintf(buf, "DELAY TIMER: 0x%02X", cpu->delay_timer_reg);
    gui_window_print(window, 1, REGISTERS_SECOND_COLUMN, buf);
    sprintf(buf, "SOUND TIMER: 0x%02X", cpu->sound_timer_reg);
    gui_window_print(window, 2, REGISTERS_SECOND_COLUMN, buf);
    sprintf(buf, "PC: 0x%04X", cpu->pc);
    gui_window_print(window, 3, REGISTERS_SECOND_COLUMN, buf);
    sprintf(buf, "SP: 0x%02X", cpu->sp);
    gui_window_print(window, 4, REGISTERS_SECOND_COLUMN, buf);
    gui_window_refresh(window);
}

