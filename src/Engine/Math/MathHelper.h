#pragma once

namespace Engine::Math::MathHelper
{
    extern float PIf;
    extern double PI;
    extern float HALF_PIf;
    extern double HALF_PI;
    extern float TAUf;
    extern double TAU;

    float DegToRad(float degrees);
    double DegToRad(double degrees);
    float RadToDeg(float radians);
    double RadToDeg(double radians);
};