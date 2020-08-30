#include <unistd.h>

#include "interpreter.h"
#include "gui.h"

int main(int argc, char **argv) {
    gui_init();
    interpreter_t *interpreter = interpreter_init();
    interpreter_run(interpreter);
    interpreter_free(interpreter);

    // gui_window_print(gui_windows_global[GUI_WINDOW_DISPLAY].window, 31, 31, "x");
    // gui_window_print(gui_windows_global[GUI_WINDOW_INSTRUCTIONS].window, 0, 0, "0x1234\tCLR\t$FBEE,$FCEE");

    sleep(10);
    endwin();

    return 0;
}