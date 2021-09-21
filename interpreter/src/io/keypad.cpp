#include <cstring>
#include "io/keypad.h"

Keypad::Keypad() {
    memset(is_key_pressed, 0, sizeof(is_key_pressed) * KEY_COUNT);

}

Keypad::~Keypad() {

}

void Keypad::set_pressed(int key_id) {
    is_key_pressed[key_id] = true;
}

void Keypad::set_released(int key_id) {
    is_key_pressed[key_id] = false;
}

bool Keypad::get_is_pressed(int key_id) {
    return is_key_pressed[key_id];
}

int Keypad::get_pressed_id() {
    int pressed = -1;
    for (int i = 0; i < KEY_COUNT; ++i) {
        if (is_key_pressed[i]) {
            pressed = i;
            break;
        }
    }
    return pressed;
}

int Keypad::get_key_count() {
    return KEY_COUNT;
}