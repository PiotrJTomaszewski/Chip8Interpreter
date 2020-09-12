#include "display.h"
#include "debug.h"

static SDL_Rect tile_ = {.w=DISPLAY_TILE_SIZE, .h=DISPLAY_TILE_SIZE};

int display_init(display_t *display) {
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result != 0) {
        debug_logf("SDL initialization error: %s", SDL_GetError());
    }
    return result;
}

int display_create_window(display_t *display) {
    display->window = SDL_CreateWindow("Chip-8 Interpreter", 100, 100, DISPLAY_SCREEN_WIDTH * DISPLAY_RESOLUTION_MULT, DISPLAY_SCREEN_HEIGHT * DISPLAY_RESOLUTION_MULT, SDL_WINDOW_SHOWN);
    if (display->window == NULL) {
        debug_logf("Error while creating SDL window: %s", SDL_GetError());
        return -1;
    }
    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
    if (display->renderer == NULL) {
        debug_logf("Error while creating SDL renderer: %s", SDL_GetError());
        return -1;
    }

    display_clear(display);
    // // On start Chip8 display should be in a random state
    // for (int y=0; y<DISPLAY_SCREEN_HEIGHT; ++y) {
    //     for (int x=0; x<DISPLAY_SCREEN_WIDTH; ++x) {
    //         display->buffer[y][x] = rand() % 2;
    //         display_draw(display, y, x, display->buffer[y][x]);
    //     }
    // }
    display_show(display);
    return 0;
}

void display_destroy_window(display_t *display) {
    if (display->renderer != NULL)
        SDL_DestroyRenderer(display->renderer);
    if (display->window != NULL)
        SDL_DestroyWindow(display->window);
}

void display_end(display_t *display) {
    SDL_Quit();
}

bool display_draw(display_t *display, int y, int x, display_pixel_state_t pixel_state) {
    // Pixels wrap around the screen
    x = x % DISPLAY_SCREEN_WIDTH;
    bool collision = (pixel_state == PIXEL_INACTIVE) && (display->buffer[y][x] == PIXEL_ACTIVE);
    display->buffer[y][x] ^= pixel_state;
    display->dirty = true;
    return collision;
}

void display_clear(display_t *display) {
    for (int y=0; y<DISPLAY_SCREEN_HEIGHT; ++y) {
        for (int x=0; x<DISPLAY_SCREEN_WIDTH; ++x) {
            display->buffer[y][x] = PIXEL_INACTIVE;
        }
    }
}

void display_show(display_t *display) {
    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
    for (int y=0; y<DISPLAY_SCREEN_HEIGHT; ++y) {
        for (int x=0; x<DISPLAY_SCREEN_WIDTH; ++x) {
            if (display->buffer[y][x] == PIXEL_ACTIVE)
                SDL_SetRenderDrawColor(display->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            else
                SDL_SetRenderDrawColor(display->renderer, 0x00, 0x00, 0x00, 0xFF);
            tile_.y = y * DISPLAY_RESOLUTION_MULT;
            tile_.x = x * DISPLAY_RESOLUTION_MULT;
            SDL_RenderFillRect(display->renderer, &tile_);
        }
    }
    SDL_RenderPresent(display->renderer);
    display->dirty = false;
}
