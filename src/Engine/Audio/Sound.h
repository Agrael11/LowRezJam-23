#pragma once

#include <iostream>
#include <SDL_mixer.h>

namespace Engine::Audio
{
    class Sound
    {
        private:
            Mix_Chunk* mChunk;
            int mChannel;
        public:
            bool Load(std::string path);
            bool Play(int loops);
            bool Play(int loops, int volume);
            bool IsPlaying();
            bool Stop();
            void Unload();
    };
};