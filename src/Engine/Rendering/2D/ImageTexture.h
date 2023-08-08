#pragma once

#include <iostream>

#include "Texture.h"
#include "../../Math/Vector2.h"

namespace Engine::Rendering
{
    class Renderer;
    class ImageTexture: public Texture
    {
    private:
    public:
        bool Load(std::string filePath, Renderer &renderer);
        void Unload();
    };
};