#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

#include "display.h"
#include "gui.h"

void display_init(display_t *display) {
    for (int i=0; i<DISPLAY_ROWS; i++) {
        for (int j=0; j<64; j++) {
            display->buffer[i][j] = 0;
        }
    }
}

void display_free(display_t *display) {
    free(display);
}

int display_draw(display_t *display, int x, int y, uint8_t byte) {
    for (int bit_pos=8; bit_pos>=0; --bit_pos) {
        display->buffer[y][(x-bit_pos+8) % DISPLAY_COLS] ^= (byte & (1 << bit_pos)) >> bit_pos;
    }

    return 0;
}

void display_show(display_t *display) {
    for (int i=0; i<DISPLAY_ROWS; i++) {
            for (int j=0; j<64; j++) {
                int use_reversed_color = display->buffer[i][j]; // boolean
                gui_window_putchar(&gui_windows_global[GUI_WINDOW_DISPLAY], i, j, ' ', use_reversed_color);
            }
    }
    gui_window_refresh(&gui_windows_global[GUI_WINDOW_DISPLAY]);
}

void display_clear(display_t *display) {
    for (int i=0; i<DISPLAY_ROWS; i++) {
        for (int j=0; j<64; j++) {
            display->buffer[i][j] = 0;
        }
    }
    display_show(display);
}