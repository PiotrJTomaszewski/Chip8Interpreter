#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdbool.h>

#define KEYPAD_KEY_COUNT 16

typedef enum KEYPAD_KEYMAP {
    KEYPAD_MAP_0 = SDLK_x,
    KEYPAD_MAP_1 = SDLK_1,
    KEYPAD_MAP_2 = SDLK_2,
    KEYPAD_MAP_3 = SDLK_3,
    KEYPAD_MAP_4 = SDLK_q,
    KEYPAD_MAP_5 = SDLK_w,
    KEYPAD_MAP_6 = SDLK_e,
    KEYPAD_MAP_7 = SDLK_a,
    KEYPAD_MAP_8 = SDLK_s,
    KEYPAD_MAP_9 = SDLK_d,
    KEYPAD_MAP_A = SDLK_z,
    KEYPAD_MAP_B = SDLK_c,
    KEYPAD_MAP_C = SDLK_4,
    KEYPAD_MAP_D = SDLK_r,
    KEYPAD_MAP_E = SDLK_f,
    KEYPAD_MAP_F = SDLK_v
} keypad_map_t;

typedef struct KEYBOARD_PRESSED {
    bool keypad[KEYPAD_KEY_COUNT];
    bool exit;
    bool pause;
} keyboard_pressed_t;

void keyboard_init(keyboard_pressed_t *pressed);
void keyboard_poll_pressed(keyboard_pressed_t *pressed);
void keyboard_wait_for_press(keyboard_pressed_t *pressed);
bool keyboard_is_keypad_key_pressed(keyboard_pressed_t *pressed, int key_id);
// Get id of the first pressed key on keypad
keypad_map_t keyboard_get_pressed(keyboard_pressed_t *pressed);
#endif //__KEYBOARD_H__
