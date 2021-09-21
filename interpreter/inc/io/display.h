#pragma once

class Display {
public:
    Display();
    ~Display();
    // Returns true if a pixel was deactivated
    bool xor_pixel(int y, int x);
    bool get_pixel(int y, int x);
    void clear();
    int get_width();
    int get_height();
    bool get_is_dirty();
    void mark_clean();

private:
    static const int DISPLAY_SCREEN_WIDTH = 64;
    static const int DISPLAY_SCREEN_HEIGHT = 32;
    bool buffer[DISPLAY_SCREEN_HEIGHT][DISPLAY_SCREEN_WIDTH]; // TODO: Use a smaller buffer
    bool is_dirty;
};
