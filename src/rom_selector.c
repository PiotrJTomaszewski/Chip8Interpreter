#include <ncurses.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <menu.h>
#include <string.h>

#include "rom_selector.h"

static ITEM *_create_menu(ITEM **menu_items) {
    int ch;
    MENU *menu;
    WINDOW *window;

    initscr();
    curs_set(0);
    noecho();

    menu = new_menu(menu_items);
    window = newwin(30, 80, 5, 5);
    box(window, 0, 0 );
    keypad(window, TRUE);

    set_menu_win(menu, window);
    set_menu_sub( menu, derwin(window, 16, 14, 1, 1));
    set_menu_mark(menu, " ");
    set_menu_format(menu, 15, 1);
    post_menu(menu);
    wrefresh(window);

    int menu_should_run = TRUE;
    ITEM *selected_item;
    while(menu_should_run) {
        ch = wgetch(window);
        switch(ch) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE:
                menu_driver(menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(menu, REQ_SCR_UPAGE);
                break;
            case 10: // Enter
                selected_item = current_item(menu);
                menu_should_run = false;
                break;
            default:
                break;
        }
        wrefresh(window);
    }
    unpost_menu(menu);
    free_menu(menu);
    return selected_item;
}

void rom_selector(char *path) {
    const char *Location = "/home/piotr/tmp/chip8_games"; // TODO: Allow modifying this
    char *rom_options;
    DIR *d;
    struct dirent *dir;

    d = opendir(Location);
    if (d) {
        int number_of_files = 0;
        int begin = telldir(d);
        while((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                ++number_of_files;
            }
        }

        MENU *rom_selector_menu;
        ITEM *cur_item;
        ITEM **rom_items;
        char **file_names;
        WINDOW *menu_window;
        menu_window = newwin(10, 40, 4, 4);
        keypad(menu_window, TRUE);
        file_names = (char**)calloc(number_of_files, sizeof(char*));
        rom_items = (ITEM **)calloc(number_of_files+1, sizeof(ITEM*));
        seekdir(d, begin);
        int i=0;
        while((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                file_names[i] = (char*)calloc(256, sizeof(char));
                strcpy(file_names[i], dir->d_name);
                rom_items[i] = new_item(file_names[i], "");
                ++i;
            }
            dir = readdir(d);
        }
        closedir(d);
        ITEM *selected = _create_menu(rom_items);
        strcpy(path, Location);
        strcat(path, "/");
        strcat(path, selected->name.str);

        for(i=0; i<number_of_files; ++i) {
            free_item(rom_items[i]);
        }
    }
}
