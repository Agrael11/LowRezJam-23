#pragma once

#include "Engine/Rendering/Sprite.h"

class SpriteManager
{
    private:
        SpriteManager();

    public:
        static bool AddSprite(std::string id, Engine::Rendering::Sprite sprite);
        static bool SpriteExists(std::string id);
        static Engine::Rendering::Sprite& GetSprite(std::string id);
        static bool RemoveSprite(std::string id);
};