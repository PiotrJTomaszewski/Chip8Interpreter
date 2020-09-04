#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "debug.h"

#define REGISTERS_SECOND_COLUMN 14

static int current_log_line = 0;
static char buffer[30];

static inline void log_to_gui(char *message) {
    gui_window_t *window = &gui_windows_global[GUI_WINDOW_LOGS];
    gui_window_print(window, current_log_line, 0, message);

    // Clear the rest of the line (in case a shorter line is overriting the longer one)
    for (int i=strlen(message); i<GUI_LOGS_WINDOW_WIDTH; ++i) {
        gui_window_putchar(window, current_log_line, i, ' ', false);
    }
    current_log_line = (current_log_line + 1) % GUI_LOGS_WINDOW_HEIGHT;
    gui_window_refresh(window);
}

void debug_error_print(char *message) {
    log_to_gui(message);
}

void debug_error_printf(char *format, ...) {
    va_list valist;
    va_start(valist, format);
    vsprintf(buffer, format, valist);
    va_end(valist);
    log_to_gui(buffer);
}

void debug_info_print(char *message) {
    log_to_gui(message);
}

void debug_info_printf(char *format, ...) {
    va_list valist;
    va_start(valist, format);
    vsprintf(buffer, format, valist);
    va_end(valist);
    log_to_gui(buffer);
}

void print_registers(cpu_t *cpu) {
    gui_window_t *window = &gui_windows_global[GUI_WINDOW_REGISTERS];
    for (int i=0; i<GENERAL_REGISTERS_NO; ++i) {
        sprintf(buffer, "%X: 0x%02X", i, cpu->general_reg[i]);
        gui_window_print(window, i, 0, buffer);
    }
    sprintf(buffer, "REGISTER I: 0x%04X", cpu->mem_addr_reg);
    gui_window_print(window, 0, REGISTERS_SECOND_COLUMN, buffer);
    sprintf(buffer, "DELAY TIMER: 0x%02X", cpu->delay_timer_reg);
    gui_window_print(window, 1, REGISTERS_SECOND_COLUMN, buffer);
    sprintf(buffer, "SOUND TIMER: 0x%02X", cpu->sound_timer_reg);
    gui_window_print(window, 2, REGISTERS_SECOND_COLUMN, buffer);
    sprintf(buffer, "PC: 0x%04X", cpu->pc);
    gui_window_print(window, 3, REGISTERS_SECOND_COLUMN, buffer);
    sprintf(buffer, "SP: 0x%02X", cpu->sp);
    gui_window_print(window, 4, REGISTERS_SECOND_COLUMN, buffer);
    gui_window_refresh(window);
}

void print_memory(memory_t *memory) {
    gui_window_t *window = &gui_windows_global[GUI_WINDOW_MEMORY];
    const max_width = GUI_MEMORY_WINDOW_WIDTH-1;
    for (int i=0; i<MEMORY_SIZE; ++i) {
        sprintf(buffer, "%02X", memory->general[i]);
        gui_window_print(window, i/max_width, i%max_width, buffer);
    }
    gui_window_refresh(window);
}
