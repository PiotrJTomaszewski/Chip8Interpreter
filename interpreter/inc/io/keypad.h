#pragma once

class Keypad {
public:
    Keypad();
    ~Keypad();
    void set_pressed(int key_id);
    bool get_is_pressed(int key_id);
    int get_pressed_id();
    void wait_for_press();

private:
    static const int KEY_COUNT = 16;
    bool is_key_pressed[KEY_COUNT];
};