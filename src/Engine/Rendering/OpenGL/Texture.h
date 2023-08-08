#pragma once

#include <iostream>

#include "../../Math/Vector2.h"

namespace Engine::Rendering
{
    class Renderer;
    class Texture
    {
    protected:
        unsigned int mTexture;
        Engine::Math::Vector2 mSize;
    public:
        int GetWidth();
        int GetHeight();
        Engine::Math::Vector2 GetSize();
        unsigned int GetHandle();
        virtual void Unload();
    };
};