#include <cstring>
#include "io/display.h"

Display::Display() {
    is_dirty = false;
}

Display::~Display() {

}

// Returns true if a pixel was deactivated
bool Display::xor_pixel(int y, int x) {
    // Pixels wrap around the screen
    x %= DISPLAY_SCREEN_WIDTH;
    y %= DISPLAY_SCREEN_HEIGHT;
    bool collision = buffer[y][x];
    buffer[y][x] ^= 1;
    is_dirty = true;
    return collision;
}

bool Display::get_pixel(int y, int x) {
    return buffer[y][x];
}

void Display::clear() {
    memset(buffer, 0, sizeof(buffer));
}

int Display::get_width() {
    return DISPLAY_SCREEN_WIDTH;
}

int Display::get_height() {
    return DISPLAY_SCREEN_HEIGHT;
}

bool Display::get_is_dirty() {
    return is_dirty;
}

void Display::mark_clean() {
    is_dirty = false;
}
