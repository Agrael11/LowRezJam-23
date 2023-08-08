#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "../../Math/Rectangle.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector2f.h"
#include "../../Math/Colorf.h"
#include "Sprite.h"
#include "Shader.h"
#include "Texture.h"

namespace Engine::Rendering
{
    class Renderer;
    class RenderTexture: public Texture
    {
    private:
        unsigned int mFrameBuffer;
    public:
        bool Create(int width, int height, Renderer &renderer);
        bool SetAsRenderTarget(Renderer &enderer);
        void Unload();
    };
};