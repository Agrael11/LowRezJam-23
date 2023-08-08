#include "Vector2.h"

#include <cmath>

#include "Vector2f.h"

#include <SDL.h>

using namespace Engine::Math;

Vector2::Vector2()
{
    this->X = 0;
    this->Y = 0;
}

Vector2::Vector2(int x, int y)
{
    this->X = x;
    this->Y = y;
}

float Vector2::Distance(Vector2 &other)
{
    int a = abs(this->X - other.X);
    int b = abs(this->Y - other.Y);
    return sqrt((float)(a * a + b * b));
}

void Vector2::Normalize()
{
    float m = GetMagnitude();;
    if (m > 0)
    {
        this->X = (int)(this->X / m);
        this->Y = (int)(this->Y / m);
    }
}

float Vector2::GetMagnitude()
{
    return sqrt((float)(this->X * this->X + this->Y * this->Y));
}

void Vector2::SetMagnitude(float magnitude)
{
    this->Normalize();
    this->X = (int)(this->X * magnitude);
    this->Y = (int)(this->Y * magnitude);
}

Vector2f Vector2::GetVector2f()
{
    return Vector2f((float)this->X, (float)this->Y);
}

Vector2 Vector2::operator+ (Vector2 const &other)
{
    return Vector2(this->X + other.X, this->Y + other.Y);
}

Vector2 Vector2::operator- (Vector2 const &other)
{
    return Vector2(this->X - other.X, this->Y - other.Y);
}

Vector2 Vector2::operator* (float mult)
{
    return Vector2((int)(this->X * mult), (int)(this->Y * mult));
}

Vector2 Vector2::operator/ (float mult)
{
    return Vector2((int)(this->X / mult), (int)(this->Y / mult));
}

SDL_Point Vector2::GetSDL_Point()
{
    SDL_Point point;
    point.x = (int)this->X;
    point.y = (int)this->Y;
    return point;
}