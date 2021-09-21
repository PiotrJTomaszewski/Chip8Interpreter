#pragma once
#include <cstdint>
#include <SDL2/SDL.h>

class Sound {
public:
    Sound();
    ~Sound();
    void play();

private:
    SDL_AudioSpec wav_spec;
    uint32_t wav_length;
    uint8_t *wav_buffer;
    SDL_AudioDeviceID device_id;
};
