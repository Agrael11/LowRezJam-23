#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Sprite.h"
#include "RenderTexture.h"
#include "Shader.h"
#include "../../Math/Color.h"
#include "../../Math/Colorf.h"

namespace Engine::Rendering
{
    class Renderer
    {
        private:
            SDL_Renderer* mRenderer;
            Shader* mActiveShader;
            Rectangle mViewport;
            bool mRenderTexture;

        public:
            SDL_Renderer* GetSDL_Renderer();

            void Init(SDL_Window &window, Uint32 flags);
            void Begin();
            void End();
            bool DrawSprite(Sprite &sprite, Rectangle &destination, Rectangle *source = NULL, double rotation = 0, bool flipHorizontal = false, bool flipVertical = false);
            void Clean(Color &color);
            void Clean(Colorf &color);
            void SetViewport(int x, int y, int width, int height);
            Rectangle GetViewport();
            bool SetRenderTarget(RenderTexture &texture);
            bool CleanRenderTarget();
            void SetActiveShader(Shader* shader);
            Shader* GetActiveShader();

            void Destroy();
    };
};