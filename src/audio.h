#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "SDL2/SDL.h"
#include <stdint.h>

typedef struct AUDIO {
    SDL_AudioSpec wav_spec;
    uint32_t wav_length;
    uint8_t *wav_buffer;
    SDL_AudioDeviceID device_id;
} audio_t;

int audio_init();
void audio_destroy();
void audio_play(int seconds);
void audio_pause();

#endif //__AUDIO_H__
