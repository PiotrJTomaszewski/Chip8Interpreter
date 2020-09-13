#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "rom_selector.h"
#include "memory.h"
#include "cpu.h"
#include "keyboard.h"
#include "display.h"
#include "debug.h"
#include "audio.h"

#define TICK_MILLISECONDS 2
#define FRAME_MILLISECONDS 17
#define OPERATIONS_PER_FRAME FRAME_MILLISECONDS/TICK_MILLISECONDS

display_t display_;
memory_t memory_;

void interpreter_loop() {
    bool should_run = true;
    cpu_t cpu;
    keyboard_pressed_t pressed_keys;
    keyboard_init(&pressed_keys);
    opcode_t opcode;
    uint32_t start, stop;
    display_create_window(&display_); // TODO: Read this error
    cpu_init(&cpu, &display_, &memory_, &pressed_keys);
    while (should_run) {
        start = SDL_GetTicks();
        keyboard_poll_pressed(&pressed_keys);
        for (int cycles=0; cycles<OPERATIONS_PER_FRAME; ++cycles) {
            cpu_fetch_next_opcode(&cpu, &opcode);
            cpu_exec_opcode(&cpu, &opcode);
        }
        if (display_.dirty) {
            display_show(&display_);
        }
        debug_print_all(&memory_, &cpu, &pressed_keys);
        if (cpu.delay_timer > 0) {
            --cpu.delay_timer;
        }
        if (cpu.sound_timer > 0) {
            --cpu.sound_timer;
        }
        stop = SDL_GetTicks();
        if (stop-start < FRAME_MILLISECONDS) {
            SDL_Delay(FRAME_MILLISECONDS - (stop - start));
        }
        if (pressed_keys.exit) should_run = false;
        while (pressed_keys.pause) {
            keyboard_wait_for_press(&pressed_keys);
        }
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc <= 1) {
        puts("Too few arguments"); // TODO: Add a proper message (with help)
        return -1;
    }
    char rom_path[256];
    rom_selector(argv[1], rom_path);
    // TODO: Check if rom was selected
    memory_load_rom_file(&memory_, rom_path);
    if (display_init(&display_) == 0) {
        debug_windows_init();
        audio_init();
        interpreter_loop();
        audio_destroy();
        debug_windows_end();
    } else {
        display_end(&display_);
    }
    return 0;
}
