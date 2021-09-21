#pragma once
#include <cstdint>

class Timers {
public:
    Timers();
    ~Timers();
    void tick();
    void restart();
    void set_delay_timer(uint8_t value);
    uint8_t get_delay_timer();
    void set_sound_timer(uint8_t value);
    uint8_t get_sound_timer();

private:
    uint8_t delay_timer;
    uint8_t sound_timer;
};
