#pragma once

class Keypad {
public:
    Keypad();
    ~Keypad();
    void set_pressed(int key_id);
    void set_released(int key_id);
    bool get_is_pressed(int key_id);
    int get_pressed_id();
    int get_key_count();

private:
    static const int KEY_COUNT = 16;
    bool is_key_pressed[KEY_COUNT];
};