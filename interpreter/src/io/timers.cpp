#include "io/timers.h"

void empty_callback() {

}

Timers::Timers() {
    delay_timer = 0;
    sound_timer = 0;
    play_sound_callback = empty_callback;
    stop_sound_callback = empty_callback;
}

Timers::~Timers() {

}

void Timers::attach_sound_callbacks(void (*play_sound)(void), void (*stop_sound)(void)) {
    play_sound_callback = play_sound;
    stop_sound_callback = stop_sound;
}

void Timers::tick() {
    if (delay_timer > 0) {
        --delay_timer;
    }
    if (sound_timer > 0) {
        --sound_timer;
    } else {
        stop_sound_callback();
    }
}

void Timers::restart() {
    delay_timer = 0;
    sound_timer = 0;
}

void Timers::set_delay_timer(uint8_t value) {
    delay_timer = value;
}

uint8_t Timers::get_delay_timer() {
    return delay_timer;
}

void Timers::set_sound_timer(uint8_t value) {
    sound_timer = value;
    play_sound_callback();
}

uint8_t Timers::get_sound_timer() {
    return sound_timer;
}
