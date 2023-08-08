#pragma once

#include <iostream>

#include <SDL_ttf.h>

namespace Engine::Rendering
{
    class Renderer;
    class TtfFont
    {
    private:
        TTF_Font* mFont;
    public:
        bool Load(std::string filePath, int size, Renderer &renderer);
        TTF_Font* GetFont();
        void Unload();
    };
};