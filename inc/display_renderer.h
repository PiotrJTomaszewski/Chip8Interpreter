#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "io/display.h"

class DisplayRenderer {
public:
    DisplayRenderer(Display &display);
    ~DisplayRenderer();
    GLuint get_texture();
    int get_width();
    int get_height();
    void render_if_dirty();

private:
    const uint32_t COLOR_PALETTE[2] = {0xFF000000, 0xFFFFFFFF};
    Display &display;
    SDL_Surface *surface;
    GLuint texture;
    int width;
    int height;

private:
    void render();
};
