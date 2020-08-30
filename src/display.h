#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdint.h>

#define DISPLAY_COLS 64
#define DISPLAY_ROWS 32

typedef struct DISPLAY {
    // uint32_t buffer[DISPLAY_ROWS][2];
    int buffer[DISPLAY_ROWS][64];
} display_t;

void display_init(display_t *display);

void display_free(display_t *display);

int display_draw(display_t *display, int x, int y, uint8_t byte);

void display_show(display_t *display);

#endif //__DISPLAY_H__
