#ifndef __GUI_H__
#define __GUI_H__

#include <ncurses.h>

#define GUI_WINDOWS_NUMBER 5
#define GUI_MAX_LOGS_NO 18

enum GUI_WINDOW_TYPES {
    GUI_WINDOW_DISPLAY,
    GUI_WINDOW_INSTRUCTIONS,
    GUI_WINDOW_REGISTERS,
    GUI_WINDOW_LOGS,
    GUI_WINDOW_MEMORY
};

typedef struct GUI_WINDOWS_TYPE {
    WINDOW *window;
    int width;
    int height;
    int start_x;
    int start_y;
    const char *title;
} gui_window_t;

extern gui_window_t gui_windows_global[];

void gui_window_print(gui_window_t *gui_window, int y, int x, const char *text);
void gui_window_putchar(gui_window_t *gui_window, int y, int x, const char character, int reverse_color);
void gui_window_refresh(gui_window_t *gui_window);
void gui_init();
void gui_destroy();

#endif //__GUI_H__
