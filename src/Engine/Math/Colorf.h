#pragma once

namespace Engine::Math
{
    class Color;
    class Colorf
    {
        public:
        float R;
        float G;
        float B;
        float A;

        Colorf();
        Colorf(float r, float g, float b, float a);
        Color GetColor();
    };
};