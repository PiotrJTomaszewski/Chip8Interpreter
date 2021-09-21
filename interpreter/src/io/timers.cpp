#include "io/timers.h"

Timers::Timers() {
    delay_timer = 0;
    sound_timer = 0;
}

Timers::~Timers() {

}

void Timers::tick() {
    if (delay_timer > 0) {
        --delay_timer;
    }
    if (sound_timer > 0) {
        --sound_timer;
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
}

uint8_t Timers::get_sound_timer() {
    return sound_timer;
}
