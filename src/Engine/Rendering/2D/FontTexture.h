#pragma once

#include <iostream>

#include "Texture.h"
#include "../../Math/Vector2.h"
#include "../../Math/Color.h"
#include "TtfFont.h"

namespace Engine::Rendering
{
    class Renderer;
    class FontTexture: public Texture
    {
    private:
        bool mCreated = false;
    public:
        bool Generate(std::string text, TtfFont font, Engine::Math::Color color, Renderer &renderer, int wrap = -1);
        void Unload();
    };
};