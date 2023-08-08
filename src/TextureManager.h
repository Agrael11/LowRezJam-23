#pragma once

#include "Engine/Rendering/ImageTexture.h"
#include <unordered_map>
#include <optional>

class TextureManager
{
    private:
        TextureManager();

    public:
        static bool LoadTexture(std::string id, std::string filename, Engine::Rendering::Renderer &renderer);
        static bool TextureExists(std::string id);
        static Engine::Rendering::ImageTexture& GetTexture(std::string id);
        static bool RemoveTexture(std::string id);
};