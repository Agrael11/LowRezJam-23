#include "../Helper/Logger.h"
#include "../Helper/format.h"

#include "Sound.h"

using namespace Engine::Helper;
using namespace Engine::Audio;

bool Sound::Load(std::string path)
{
    this->mChunk = Mix_LoadWAV(path.c_str());
    if (this->mChunk == NULL)
    {
        Logger::Log(Logger::Error, string_format("Failed to load sound chunk file %s! SDL_Mixer Error: %s", path.c_str(), Mix_GetError()));
        return false;
    }
    return true;
}

bool Sound::Play(int loops)
{
    int channel = Mix_PlayChannel(-1, this->mChunk, loops);
    if (channel == -1)
    {
        Logger::Log(Logger::Error, string_format("Failed to play sound! SDL_Mixer Error: %s", Mix_GetError()));
        return false;
    }
    this->mChannel = channel;
    return true;
}

bool Sound::Play(int loops, int volume)
{
    int channel = Mix_PlayChannel(-1, this->mChunk, loops);
    if (channel == -1)
    {
        Logger::Log(Logger::Error, string_format("Failed to play sound! SDL_Mixer Error: %s", Mix_GetError()));
        return false;
    }
    Mix_Volume(channel, volume);
    this->mChannel = channel;
    return true;
}

bool Sound::IsPlaying()
{
    if (Mix_Playing(this->mChannel) == 0)
    {
        return false;
    }
    return true;
}

bool Sound::Stop()
{
    if (Mix_HaltChannel(this->mChannel) == 0)
    {
        Logger::Log(Logger::Error, string_format("Failed to stop sound! SDL_Mixer Error: %s", Mix_GetError()));
        return false;
    }
    return true;
}

void Sound::Unload()
{
    if (this->mChunk == NULL)
    {
        return;   
    }
    Mix_FreeChunk(this->mChunk);
}