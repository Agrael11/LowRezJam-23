#pragma once

#include <iostream>
#include <SDL_mixer.h>

namespace Engine::Audio
{
    class Music
    {
        private:
            Mix_Music* mMusic;
        public:
            bool Load(std::string path);
            void SetVolume(int volume);
            bool Play(int loops);
            bool Play(int loops, bool override, int fadeout);
            bool Pause();
            bool Rewind();
            bool IsPlaying();
            bool Resume();
            bool Stop(int fadeout);
            void Unload();
    };
};