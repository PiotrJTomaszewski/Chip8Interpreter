#include "audio.h"
#include "debug.h"

static audio_t audio_;

int audio_init() {
    if (SDL_LoadWAV("../chip8_sound.wav", &audio_.wav_spec, &audio_.wav_buffer, &audio_.wav_length) == NULL) {
        debug_logf("Error, couldn't load chip8_sound.wav: %s", SDL_GetError());
        return -1;
    } else {
        audio_.device_id = SDL_OpenAudioDevice(NULL, 0, &audio_.wav_spec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if (audio_.device_id == 0) {
            debug_logf("Error, couldn't connect to audio device: %s", SDL_GetError());
            return -1;
        }
    }
    return 0;
}

void audio_destroy() {
    if (audio_.device_id != 0)
        SDL_CloseAudioDevice(audio_.device_id);
    if (audio_.wav_buffer != NULL)
        SDL_FreeWAV(audio_.wav_buffer);
}

void audio_play(int seconds) {
    for (int i=0; i<seconds; i++) {
        SDL_QueueAudio(audio_.device_id, audio_.wav_buffer, audio_.wav_length); // TODO: Handle errors
    }
    SDL_PauseAudioDevice(audio_.device_id, 0);
}

void audio_pause() {
    SDL_PauseAudioDevice(audio_.device_id, 1);
}
