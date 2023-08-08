#pragma once

#include <SDL.h>

namespace Engine::Math
{
    class Colorf;
    class Color
    {
        public:
        int R;
        int G;
        int B;
        int A;

        Color();
        Color(int r, int g, int b, int a);

        Colorf GetColorf();
        SDL_Color GetSDL_Color();
    };
};