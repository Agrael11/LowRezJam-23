#pragma once

#include <SDL_mixer.h>

namespace Engine::Audio
{
    bool InitAudio(int frequency, Uint16 format, int channels, int chunksize);
    void QuitAudio();
};