#pragma once

#include <SDL.h>

namespace Engine::Math
{
    class Rectangle
    {
        public:
        int X;
        int Y;
        int Width;
        int Height;

        Rectangle();
        Rectangle(int x, int y, int width, int height);
        
        bool Intersects(Rectangle &other);
        bool Contains(Rectangle &other);
        int GetX2();
        int GetY2();

        SDL_Rect GetSDL_Rect();
    };
};