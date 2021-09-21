#include <cstring>
#include "io/display.h"

Display::Display() {
    dirty = false;
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
    dirty = true;
    return collision;
}

void Display::clear() {
    memset(buffer, 0, sizeof(buffer));
}
