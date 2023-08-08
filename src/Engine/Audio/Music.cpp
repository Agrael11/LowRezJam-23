#include "../Helper/Logger.h"
#include "../Helper/format.h"

#include "Music.h"

using namespace Engine::Helper;
using namespace Engine::Audio;

bool Music::Load(std::string path)
{
    this->mMusic = Mix_LoadMUS(path.c_str());
    if (this->mMusic == NULL)
    {
        Logger::Log(Logger::Error, string_format("Failed to load music file %s! SDL_Mixer Error: %s", path.c_str(), Mix_GetError()));
        return false;
    }
    return true;
}

void Music::SetVolume(int volume)
{
    Mix_VolumeMusic(volume);
}

bool Music::Play(int loops)
{
    if (Mix_PlayingMusic())
    {
        Logger::Log(Logger::Error, "Music is already playing.");
        return false;
    }
    if (Mix_PlayMusic(this->mMusic, loops) != 0)
    {
        Logger::Log(Logger::Error, string_format("Failed to play music! SDL_Mixer Error: %s", Mix_GetError()));
        return false;
    }
    return true;
}

bool Music::Play(int loops, bool override, int fadeout)
{
    if (Mix_PlayingMusic())
    {
        if (!override)
        {
            Logger::Log(Logger::Error, "Music is already playing.");
            return false;
        }

        if (fadeout > 0)
        {
            Mix_FadeOutMusic(fadeout);
        }
        else
        {
            Mix_HaltMusic();
        }
    }
    if (Mix_PlayMusic(this->mMusic, loops) != 0)
    {
        Logger::Log(Logger::Error, string_format("Failed to play music! SDL_Mixer Error: %s", Mix_GetError()));
        return false;
    }
    return true;
}

bool Music::Pause()
{
    if (Mix_PlayingMusic() == 0)
    {
        Logger::Log(Logger::Error, "No music is playing.");
        return false;
    }
    if (Mix_PausedMusic())
    {
        Logger::Log(Logger::Info, "Music already paused.");
        return true;
    }
    Mix_PauseMusic();
    return true;
}

bool Music::Rewind()
{
    if (Mix_PlayingMusic() == 0)
    {
        Logger::Log(Logger::Error, "No music is playing.");
        return false;
    }
    Mix_RewindMusic();
    return true;
}

bool Music::IsPlaying()
{
    if (Mix_PlayingMusic() == 0)
    {
        return false;
    }
    if (Mix_PausedMusic() == 1)
    {
        return false;
    }
    return true;

}

bool Music::Resume()
{
    if (Mix_PlayingMusic() == 0)
    {
        Logger::Log(Logger::Error, "No music is playing.");
        return false;
    }
    if (Mix_PausedMusic() == 0)
    {
        Logger::Log(Logger::Info, "Music is not paused.");
        return true;
    }
    Mix_ResumeMusic();
    return true;
}

bool Music::Stop(int fadeout)
{
    if (Mix_PlayingMusic() == 0)
    {
        Logger::Log(Logger::Error, "No music is playing.");
        return false;
    }
    if (fadeout > 0)
    {
        Mix_FadeOutMusic(fadeout);
        return true;
    }
    Mix_HaltMusic();
    return true;
}

void Music::Unload()
{
    if (this->mMusic == NULL)
    {
        return;   
    }
    Mix_FreeMusic(this->mMusic);
}