#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdint.h>

#define KEYBOARD_NO_KEY_PRESSED 0xFF

void keyboard_init();
uint8_t keyboard_get_pressed_key(int blocking);

#endif //__KEYBOARD_H__
