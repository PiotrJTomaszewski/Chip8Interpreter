#pragma once
#include <cstdint>

class Display {
public:
    Display();
    ~Display();
    // Returns true if a pixel was deactivated
    bool xor_pixel(int y, int x);
    bool get_pixel(int y, int x);
    uint32_t get_col(int x);
    void clear();
    int get_width();
    int get_height();
    bool get_is_dirty();
    void mark_clean();

private:
    static const int DISPLAY_SCREEN_WIDTH = 64;
    static const int DISPLAY_SCREEN_HEIGHT = 32;
    // Every 32bit int stores screen state for a single column
    uint32_t buffer[DISPLAY_SCREEN_WIDTH];
    bool is_dirty;
};
