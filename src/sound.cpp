#include "sound.h"
#include "interpreter_exception.h"

Sound::Sound() {
    if (SDL_LoadWAV("./sound_effect.wav", &wav_spec, &wav_buffer, &wav_length) == NULL) {
        printf("Error, couldn't load chip8_sound.wav: %s", SDL_GetError());
    } else {
        device_id = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if (device_id == 0) {
            printf("Error, couldn't connect to audio device: %s", SDL_GetError());
        }
    }
}

Sound::~Sound() {
    if (device_id != 0)
        SDL_CloseAudioDevice(device_id);
    if (wav_buffer != NULL)
        SDL_FreeWAV(wav_buffer);
}

void Sound::play() {
    SDL_QueueAudio(device_id, wav_buffer, wav_length);
    SDL_PauseAudioDevice(device_id, 0);
}
