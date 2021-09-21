#include <cstdint>
#include "display_renderer.h"

DisplayRenderer::DisplayRenderer(Display &display): display(display) {
    glGenTextures(1, &texture);
    width = display.get_width();
    height = display.get_height();
    surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

DisplayRenderer::~DisplayRenderer() {
    SDL_FreeSurface(surface);
    glDeleteTextures(1, &texture);
}

void DisplayRenderer::render_if_dirty() {
    if (display.get_is_dirty()) {
        render();
    }
}

void DisplayRenderer::render() {
    uint32_t *pixel_data = reinterpret_cast<uint32_t *>(surface->pixels);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            pixel_data[y*width + x] = COLOR_PALETTE[display.get_pixel(y, x)];
        }
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glDisable(GL_TEXTURE_2D);
    display.mark_clean();
}

GLuint DisplayRenderer::get_texture() {
    return texture;
}

int DisplayRenderer::get_width() {
    return width;
}

int DisplayRenderer::get_height() {
    return height;
}
