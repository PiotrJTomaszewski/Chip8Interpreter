#include <unistd.h>

#include "interpreter.h"
#include "gui.h"
#include "rom_selector.h"

int main(int argc, char **argv) {
    char rom_path[256];
    gui_init();
    rom_selector(rom_path);
    gui_init_windows();
    interpreter_t *interpreter = interpreter_init(rom_path);
    interpreter_run(interpreter);
    interpreter_free(interpreter);
    gui_destroy();
    return 0;
}