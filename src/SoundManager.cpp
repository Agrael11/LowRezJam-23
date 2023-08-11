#include "SoundManager.h"
#include "Engine/Helper/Format.h"
#include "Engine/Helper/Logger.h"

using namespace std;
using namespace Engine::Helper;

static std::unordered_map<std::string, Engine::Audio::Sound> mSounds;
static std::unordered_map<std::string, Engine::Audio::Music> mMusics;

SoundManager::SoundManager()
{
    
}

bool SoundManager::LoadSound(string id, std::string filename)
{
    if (SoundManager::SoundExists(id))
    {
        Logger::Log(Logger::Error, string_format("Sound %s already loaded", id.c_str()));
        return false;
    }

    Engine::Audio::Sound sound;
    sound.Load(filename);
    mSounds[id] = sound;

    Logger::Log(Logger::Info, string_format("Added sound %s to Sound Manager", id.c_str()));
    return true;
}

bool SoundManager::LoadMusic(string id, std::string filename)
{
    if (SoundManager::MusicExists(id))
    {
        Logger::Log(Logger::Error, string_format("Music %s already loaded", id.c_str()));
        return false;
    }

    Engine::Audio::Music music;
    music.Load(filename);
    mMusics[id] = music;

    Logger::Log(Logger::Info, string_format("Added music %s to Sound Manager", id.c_str()));
    return true;
}

bool SoundManager::SoundExists(string id)
{
    return (mSounds.count(id) > 0);
}

bool SoundManager::MusicExists(string id)
{
    return (mMusics.count(id) > 0);
}

Engine::Audio::Sound& SoundManager::GetSound(string id)
{
    if (!SoundManager::SoundExists(id))
    {
        Logger::Log(Logger::Error, string_format("Sound %s doesn't exist", id.c_str()));
        throw std::runtime_error(string_format("Sound %s doesn't exist", id.c_str()));
    }

    return mSounds[id];
}

Engine::Audio::Music& SoundManager::GetMusic(string id)
{
    if (!SoundManager::MusicExists(id))
    {
        Logger::Log(Logger::Error, string_format("Music %s doesn't exist", id.c_str()));
        throw std::runtime_error(string_format("Music %s doesn't exist", id.c_str()));
    }

    return mMusics[id];
}

bool SoundManager::RemoveSound(string id)
{
    if (!SoundManager::SoundExists(id))
    {
        Logger::Log(Logger::Error, string_format("Sound %s doesn't exist", id.c_str()));
        return false;
    }
    
    Engine::Audio::Sound sound = GetSound(id);
    sound.Unload();

    mSounds.erase(id);
    return true;
}

bool SoundManager::RemoveMusic(string id)
{
    if (!SoundManager::MusicExists(id))
    {
        Logger::Log(Logger::Error, string_format("Music %s doesn't exist", id.c_str()));
        return false;
    }
    
    Engine::Audio::Music music = GetMusic(id);
    music.Unload();

    mMusics.erase(id);
    return true;
}