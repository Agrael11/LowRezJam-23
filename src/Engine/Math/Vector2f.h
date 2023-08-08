#pragma once

#include <SDL.h>

namespace Engine::Math
{
    class Vector2;
    class Vector2f
    {
        public:
        float X;
        float Y;

        Vector2f();
        Vector2f(float x, float y);

        float Distance(Vector2f &other);
        void Normalize();
        float GetMagnitude();
        void SetMagnitude(float magnitude);
        float GetHeading();
        void SetHeading(float angle);
        Vector2 GetVector2();

        Vector2f operator+ (Vector2f const &other);
        Vector2f operator- (Vector2f const &other);
        Vector2f operator* (float mult);
        Vector2f operator/ (float mult);

        SDL_Point GetSDL_Point();
    };
};