#pragma once

#include "Engine/Audio/Sound.h"
#include "Engine/Audio/Music.h"
#include <unordered_map>
#include <optional>

class SoundManager
{
    private:
        SoundManager();

    public:
        static bool LoadMusic(std::string id, std::string filename);
        static bool LoadSound(std::string id, std::string filename);
        static bool MusicExists(std::string id);
        static bool SoundExists(std::string id);
        static Engine::Audio::Music& GetMusic(std::string id);
        static Engine::Audio::Sound& GetSound(std::string id);
        static bool RemoveMusic(std::string id);
        static bool RemoveSound(std::string id);
};