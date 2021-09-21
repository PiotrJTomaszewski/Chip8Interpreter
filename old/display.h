#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdbool.h>
#include "SDL2/SDL.h"

#define DISPLAY_RESOLUTION_MULT 10
#define DISPLAY_SCREEN_WIDTH 64
#define DISPLAY_SCREEN_HEIGHT 32
#define DISPLAY_TILE_SIZE DISPLAY_RESOLUTION_MULT

typedef enum DISPLAY_PIXEL_STATE {
    PIXEL_INACTIVE,
    PIXEL_ACTIVE
} display_pixel_state_t;

typedef struct DISPLAY_STRUCT {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int buffer[DISPLAY_SCREEN_HEIGHT][DISPLAY_SCREEN_WIDTH]; // TODO: Use a smaller buffer
    bool dirty;
} display_t;

int display_init(display_t *display);
int display_create_window(display_t *display);
void display_destroy_window(display_t *display);
void display_end(display_t *display);
// Return true if a pixel was deactivated
bool display_draw(display_t *display, int y, int x);
void display_clear(display_t *display);
void display_show(display_t *display);

#endif //__DISPLAY_H__
