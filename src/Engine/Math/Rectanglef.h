#pragma once

#include <SDL.h>

namespace Engine::Math
{
    class Rectanglef
    {
        public:
        float X;
        float Y;
        float Width;
        float Height;

        Rectanglef();
        Rectanglef(float x, float y, float width, float height);
        
        bool Intersects(Rectanglef &other);
        bool Contains(Rectanglef &other);
        float GetX2();
        float GetY2();

        SDL_Rect GetSDL_Rect();
    };
};