#include "io/keypad.h"

Keypad::Keypad() {

}

Keypad::~Keypad() {

}

void Keypad::set_pressed(int key_id) {
    is_key_pressed[key_id];
}

bool Keypad::get_is_pressed(int key_id) {
    return is_key_pressed[key_id];
}

int Keypad::get_pressed_id() {

}

void Keypad::wait_for_press() {

}
