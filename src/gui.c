#include "gui.h"

gui_window_t gui_windows_global[] = {
    // GUI_WINDOW_DISPLAY
    {.width=64, .height=32, .start_x=32+2, .start_y=0, .title="Display"},
    // GUI_WINDOW_INSTRUCTIONS
    {.width=32, .height=32, .start_x=0, .start_y=0, .title="Instructions"},
    // GUI_WINDOW_REGISTERS
    {.width=32+2, .height=18, .start_x=0, .start_y=32+2, .title="Registers"},
    // GUI_WINDOW_LOGS
    {.width=64, .height=18, .start_x=32+2, .start_y=32+2, .title="Logs"},
    // GUI_WINDOW_MEMORY
    {.width=90, .height=52, .start_x=64+2+32+2, .start_y=0, .title="Memory"},
};

void gui_window_print(gui_window_t *gui_window, int y, int x, const char *text) {
    mvwprintw(gui_window->window, y+1, x+1, text);
}

void gui_window_putchar(gui_window_t *gui_window, int y, int x, const char character, int reverse_color) {
    WINDOW *win = gui_window->window;
    if (reverse_color) {
        wattron(win, COLOR_PAIR(1));
        mvwaddch(win, y+1, x+1, character);
        wattroff(win, COLOR_PAIR(1));
    } else {
        mvwaddch(win, y+1, x+1, character);
    }
}

void gui_window_refresh(gui_window_t *gui_window) {
    wrefresh(gui_window->window);
}

static void _create_window(gui_window_t *gui_window) {
    WINDOW *win = newwin(gui_window->height+2, gui_window->width+2, gui_window->start_y, gui_window->start_x);
    refresh();
    box(win, 0, 0);
    mvwprintw(win, 0, 1, gui_window->title);
    wrefresh(win);
    gui_window->window = win;
}

void gui_init() {
    initscr();
    noecho();
    curs_set(FALSE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    refresh();
    for (int i=0; i<GUI_WINDOWS_NUMBER; ++i) {
        _create_window(&gui_windows_global[i]);
    }
}

void gui_destroy() {
    for (int i=0; i<GUI_WINDOWS_NUMBER; ++i) {
        delwin(gui_windows_global[i].window);
    }
    endwin();
}
