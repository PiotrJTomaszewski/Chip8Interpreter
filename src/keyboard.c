#include <ncurses.h>

#include "keyboard.h"
#include "gui.h"

#define KEYBOARD_KEY_NO 16
// Not in an array so I can use it in switch
#define KEYBOARD_MAP_0 'x'
#define KEYBOARD_MAP_1 '1'
#define KEYBOARD_MAP_2 '2'
#define KEYBOARD_MAP_3 '3'
#define KEYBOARD_MAP_4 'q'
#define KEYBOARD_MAP_5 'w'
#define KEYBOARD_MAP_6 'e'
#define KEYBOARD_MAP_7 'a'
#define KEYBOARD_MAP_8 's'
#define KEYBOARD_MAP_9 'd'
#define KEYBOARD_MAP_A 'z'
#define KEYBOARD_MAP_B 'c'
#define KEYBOARD_MAP_C '4'
#define KEYBOARD_MAP_D 'r'
#define KEYBOARD_MAP_E 'f'
#define KEYBOARD_MAP_F 'v'


void keyboard_init() {
    WINDOW *window = gui_windows_global[GUI_WINDOW_DISPLAY].window;
    wtimeout(window, 0);
    keypad(window, TRUE);
}

uint8_t keyboard_get_pressed_key() {
    char c = wgetch(gui_windows_global[GUI_WINDOW_DISPLAY].window);
    int pressed_key;
    switch(c) {
        case KEYBOARD_MAP_0: pressed_key = 0;  break;
        case KEYBOARD_MAP_1: pressed_key = 1;  break;
        case KEYBOARD_MAP_2: pressed_key = 2;  break;
        case KEYBOARD_MAP_3: pressed_key = 3;  break;
        case KEYBOARD_MAP_4: pressed_key = 4;  break;
        case KEYBOARD_MAP_5: pressed_key = 5;  break;
        case KEYBOARD_MAP_6: pressed_key = 6;  break;
        case KEYBOARD_MAP_7: pressed_key = 7;  break;
        case KEYBOARD_MAP_8: pressed_key = 8;  break;
        case KEYBOARD_MAP_9: pressed_key = 9;  break;
        case KEYBOARD_MAP_A: pressed_key = 10; break;
        case KEYBOARD_MAP_B: pressed_key = 11; break;
        case KEYBOARD_MAP_C: pressed_key = 12; break;
        case KEYBOARD_MAP_D: pressed_key = 13; break;
        case KEYBOARD_MAP_E: pressed_key = 14; break;
        case KEYBOARD_MAP_F: pressed_key = 15; break;
        default:             pressed_key = KEYBOARD_NO_KEY_PRESSED; break;
    }
    return pressed_key;
}
