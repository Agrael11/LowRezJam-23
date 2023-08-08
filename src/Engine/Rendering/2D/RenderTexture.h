#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "../../Math/Rectangle.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector2f.h"
#include "../../Math/Color.h"

#include "Texture.h"

namespace Engine::Rendering
{
    class Renderer;
    class RenderTexture: public Texture
    {
    private:
        SDL_TextureAccess mAccess;
    public:

        bool Create(int width, int height, Renderer &renderer);
        bool SetAsRenderTarget(Renderer &enderer);
        void Unload();
    };
};