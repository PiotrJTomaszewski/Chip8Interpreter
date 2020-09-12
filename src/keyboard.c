#include <SDL2/SDL.h>
#include <string.h>

#include "keyboard.h"

#define PAUSE_KEY_ SDLK_SPACE

void keyboard_init(keyboard_pressed_t *pressed) {
    memset(pressed->keypad, false, KEYPAD_KEY_COUNT);
    pressed->exit = false;
    pressed->pause = false;
}

static void interpret_last_event(SDL_Event *event, keyboard_pressed_t *pressed) {
    switch (event->type) {
        case SDL_QUIT:
            pressed->exit = true;
            break;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case KEYPAD_MAP_0:
                    pressed->keypad[0] = true;
                    break;
                case KEYPAD_MAP_1:
                    pressed->keypad[1] = true;
                    break;
                case KEYPAD_MAP_2:
                    pressed->keypad[2] = true;
                    break;
                case KEYPAD_MAP_3:
                    pressed->keypad[3] = true;
                    break;
                case KEYPAD_MAP_4:
                    pressed->keypad[4] = true;
                    break;
                case KEYPAD_MAP_5:
                    pressed->keypad[5] = true;
                    break;
                case KEYPAD_MAP_6:
                    pressed->keypad[6] = true;
                    break;
                case KEYPAD_MAP_7:
                    pressed->keypad[7] = true;
                    break;
                case KEYPAD_MAP_8:
                    pressed->keypad[8] = true;
                    break;
                case KEYPAD_MAP_9:
                    pressed->keypad[9] = true;
                    break;
                case KEYPAD_MAP_A:
                    pressed->keypad[10] = true;
                    break;
                case KEYPAD_MAP_B:
                    pressed->keypad[11] = true;
                    break;
                case KEYPAD_MAP_C:
                    pressed->keypad[12] = true;
                    break;
                case KEYPAD_MAP_D:
                    pressed->keypad[13] = true;
                    break;
                case KEYPAD_MAP_E:
                    pressed->keypad[14] = true;
                    break;
                case KEYPAD_MAP_F:
                    pressed->keypad[15] = true;
                    break;
                case PAUSE_KEY_:
                    pressed->pause = true;
                    break;
                default:
                    break;
                }
            break;
        case SDL_KEYUP:
            switch (event->key.keysym.sym) {
                case KEYPAD_MAP_0:
                    pressed->keypad[0] = false;
                    break;
                case KEYPAD_MAP_1:
                    pressed->keypad[1] = false;
                    break;
                case KEYPAD_MAP_2:
                    pressed->keypad[2] = false;
                    break;
                case KEYPAD_MAP_3:
                    pressed->keypad[3] = false;
                    break;
                case KEYPAD_MAP_4:
                    pressed->keypad[4] = false;
                    break;
                case KEYPAD_MAP_5:
                    pressed->keypad[5] = false;
                    break;
                case KEYPAD_MAP_6:
                    pressed->keypad[6] = false;
                    break;
                case KEYPAD_MAP_7:
                    pressed->keypad[7] = false;
                    break;
                case KEYPAD_MAP_8:
                    pressed->keypad[8] = false;
                    break;
                case KEYPAD_MAP_9:
                    pressed->keypad[9] = false;
                    break;
                case KEYPAD_MAP_A:
                    pressed->keypad[10] = false;
                    break;
                case KEYPAD_MAP_B:
                    pressed->keypad[11] = false;
                    break;
                case KEYPAD_MAP_C:
                    pressed->keypad[12] = false;
                    break;
                case KEYPAD_MAP_D:
                    pressed->keypad[13] = false;
                    break;
                case KEYPAD_MAP_E:
                    pressed->keypad[14] = false;
                    break;
                case KEYPAD_MAP_F:
                    pressed->keypad[15] = false;
                    break;
                case PAUSE_KEY_:
                    pressed->pause = false;
                    break;
                default:
                    break;
                }
            break;
        default:
            break;
    }
}

void keyboard_poll_pressed(keyboard_pressed_t *pressed) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        interpret_last_event(&event, pressed);
    }
}

void keyboard_wait_for_press(keyboard_pressed_t *pressed) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    interpret_last_event(&event, pressed);
}

bool keyboard_is_keypad_key_pressed(keyboard_pressed_t *pressed, int key_id) {
    return pressed->keypad[key_id];
}

keypad_map_t keyboard_get_pressed(keyboard_pressed_t *pressed) {
    for (int i=0; i<KEYPAD_KEY_COUNT; ++i) {
        if (pressed->keypad[i]) {
            return i;
        }
    }
}
