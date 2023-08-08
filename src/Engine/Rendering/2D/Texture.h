#pragma once

#include <iostream>

#include <SDL.h>

#include "../../Math/Vector2.h"

namespace Engine::Rendering
{
    class Renderer;
    class Texture
    {
    protected:
        SDL_Texture* mTexture;
        Engine::Math::Vector2 mSize;
    public:
        int GetWidth();
        int GetHeight();
        Engine::Math::Vector2 GetSize();
        SDL_Texture* GetHandle();
        virtual void Unload();
    };
};