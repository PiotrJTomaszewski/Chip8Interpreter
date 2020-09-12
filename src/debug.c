#include <stdarg.h>
#include <stdio.h>
#include <ncurses.h>

#include "debug.h"

#define DEBUG_WINDOW_COUNT_ 5

enum DEBUG_WINDOW_TYPE {
    DEBUG_WINDOW_LOGS,
    DEBUG_WINDOW_MEMORY,
    DEBUG_WINDOW_STACK,
    DEBUG_WINDOW_REGISTERS,
    DEBUG_WINDOW_KEYBOARD,
};

static debug_window_t debug_windows_[] = {
    // DEBUG_WINDOW_LOGS
    {.width=64, .height=31, .start_x=0, .start_y=0, .title="Logs"},
    // DEBUG_WINDOW_MEMORY
    {.width=156, .height=29, .start_x=0, .start_y=31, .title="Memory"},
    // DEBUG_WINDOW_STACK
    {.width=32, .height=13, .start_x=64, .start_y=18, .title="Stack"},
    // DEBUG_WINDOW_REGISTERS
    {.width=32, .height=18, .start_x=64, .start_y=0, .title="Registers"},
    // DEBUG_WINDOW_KEYBOARD
    {.width=10, .height=10, .start_x=96, .start_y=0, .title="Keyboard"},
};

// Starts with one because window border is treated as line 0
static int next_log_line_ = 1;

static char buffer_[100];

void debug_windows_init() {
    initscr();
    noecho();
    curs_set(FALSE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    WINDOW *win;
    for (int i=0; i<DEBUG_WINDOW_COUNT_; ++i) {
        win = newwin(debug_windows_[i].height, debug_windows_[i].width, debug_windows_[i].start_y, debug_windows_[i].start_x);
        refresh();
        box(win, 0, 0);
        mvwprintw(win, 0, 1, debug_windows_[i].title);
        wrefresh(win);
        debug_windows_[i].window = win;
    }
}

void debug_windows_end() {
    for (int i=0; i<DEBUG_WINDOW_COUNT_; ++i) {
        delwin(debug_windows_[i].window);
    }
    endwin();
}

void debug_log(const char *msg) {
    mvwprintw(debug_windows_[DEBUG_WINDOW_LOGS].window, next_log_line_, 1, msg);
    next_log_line_ = next_log_line_ % (debug_windows_[DEBUG_WINDOW_LOGS].height-2) + 1;
    wrefresh(debug_windows_[DEBUG_WINDOW_LOGS].window);
}

void debug_logf(const char *format, ...) {
    va_list valist;
    va_start(valist, format);
    vsprintf(buffer_, format, valist);
    va_end(valist);
    debug_log(buffer_);
}

void debug_print_memory(memory_t *memory) {
    WINDOW *win = debug_windows_[DEBUG_WINDOW_MEMORY].window;
    const int max_width = debug_windows_[DEBUG_WINDOW_MEMORY].width-3;
    for (int i=0; i<MEMORY_SIZE; ++i) {
        sprintf(buffer_, "%02X", memory_read_byte(memory, i));
        mvwprintw(win, i / max_width + 1, i % max_width + 1, buffer_);
    }
    wrefresh(win);
}

void debug_print_stack(cpu_t *cpu) {
    int col;
    for (int i=0; i<cpu->sp; i++) {
        sprintf(buffer_, "%d: 0x%04X", i, cpu->stack[i]);
        col = (i < debug_windows_[DEBUG_WINDOW_STACK].height-2) ? 1 : 14;
        mvwprintw(debug_windows_[DEBUG_WINDOW_STACK].window, i + 1, col, buffer_);
    }
    wrefresh(debug_windows_[DEBUG_WINDOW_STACK].window);
}

void debug_print_registers(cpu_t *cpu) {
    const int RegistersSecondColumn = 13;
    WINDOW *win = debug_windows_[DEBUG_WINDOW_REGISTERS].window;
    for (int i=0; i<CPU_GENERAL_REGISTERS_COUNT; ++i) {
        sprintf(buffer_, "%X: 0x%02X", i, cpu->general_reg[i]);
        mvwprintw(win, i+1, 1, buffer_);
    }
    sprintf(buffer_, "REGISTER I: 0x%04X", cpu->mem_addr_reg);
    mvwprintw(win, 1, RegistersSecondColumn, buffer_);
    sprintf(buffer_, "DELAY TIMER: 0x%02X", cpu->delay_timer.reg);
    mvwprintw(win, 2, RegistersSecondColumn, buffer_);
    sprintf(buffer_, "SOUND TIMER: 0x%02X", cpu->sound_timer.reg);
    mvwprintw(win, 3, RegistersSecondColumn, buffer_);
    sprintf(buffer_, "PC: 0x%04X", cpu->pc);
    mvwprintw(win, 4, RegistersSecondColumn, buffer_);
    sprintf(buffer_, "SP: 0x%02X", cpu->sp);
    mvwprintw(win, 5, RegistersSecondColumn, buffer_);
    wrefresh(win);
}

static void debug_print_key_status(WINDOW *win, char key_symbol, bool status, int y, int x) {
    if (status) {
        wattron(win, COLOR_PAIR(1));
        mvwaddch(win, y, x, key_symbol);
        wattroff(win, COLOR_PAIR(1));
    } else {
        mvwaddch(win, y, x, key_symbol);
    }
}

void debug_print_pressed_keys(keyboard_pressed_t *pressed) {
    WINDOW *win = debug_windows_[DEBUG_WINDOW_KEYBOARD].window;
    // Keys 1-9
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            debug_print_key_status(win, '0'+3*i+j+1, keyboard_is_keypad_key_pressed(pressed, 3*i+j+1), 2*i+1, 2*j+1);
        }
    }
    // Keys C-F
    for (int i=0; i<4; ++i) {
        debug_print_key_status(win, 'C'+i, keyboard_is_keypad_key_pressed(pressed, 0xC+i), 2*i+1, 7);
    }
    // Keys A, 0, B
    debug_print_key_status(win, 'A', keyboard_is_keypad_key_pressed(pressed, 0xA), 7, 1);
    debug_print_key_status(win, '0', keyboard_is_keypad_key_pressed(pressed, 0x0), 7, 3);
    debug_print_key_status(win, 'B', keyboard_is_keypad_key_pressed(pressed, 0xB), 7, 5);
    wrefresh(win);
}

void debug_print_all(memory_t *memory, cpu_t *cpu, keyboard_pressed_t *pressed) {
    debug_print_memory(memory);
    debug_print_stack(cpu);
    debug_print_registers(cpu);
    debug_print_pressed_keys(pressed);
}
