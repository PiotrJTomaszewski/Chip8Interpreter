#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

#include "display.h"

void display_init(display_t *display) {
    for (int i=0; i<DISPLAY_ROWS; i++) {
        for (int j=0; j<64; j++) {
            display->buffer[i][j] = 0;
        }
    }
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
}

void display_free(display_t *display) {
    endwin();
    free(display);
}

int display_draw(display_t *display, int x, int y, uint8_t byte) {
    for (int bit_pos=8; bit_pos>=0; --bit_pos) {
        display->buffer[y][x-bit_pos+8] ^= (byte & (1 << bit_pos)) >> bit_pos;
    }

    return 0;
}

void display_show(display_t *display) {
    char c;
    c = ' ';
    for (int i=0; i<DISPLAY_ROWS; i++) {
            for (int j=0; j<64; j++) {
                if (display->buffer[i][j] == 1) {
                    attron(COLOR_PAIR(1));
                    mvaddch(i, j, c);
                    attroff(COLOR_PAIR(1));
                } else {
                    mvaddch(i, j, c);
                }
            }
    }
    refresh();
}