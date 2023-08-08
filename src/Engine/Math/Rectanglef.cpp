#include "Rectanglef.h"

#include <SDL.h>

using namespace Engine::Math;

Rectanglef::Rectanglef()
{
    this->X = 0;
    this->Y = 0;
    this->Width = 0;
    this->Height = 0;
}

Rectanglef::Rectanglef(float x, float y, float width, float height)
{
    this->X = x;
    this->Y = y;
    this->Width = width;
    this->Height = height;
}

bool Rectanglef::Intersects(Rectanglef &other)
{
    return (
        this->GetX2() > other.X &&
        this->X < other.GetX2() &&
        this->GetY2() > other.Y &&
        this->Y < other.GetY2());
}

bool Rectanglef::Contains(Rectanglef &other)
{
    return (
        this->X > other.X &&
        this->GetX2() < other.GetX2() &&
        this->Y > other.Y &&
        this->GetY2() < other.GetY2());
}

float Rectanglef::GetX2()
{
    return this->X + this->Width;
}

float Rectanglef::GetY2()
{
    return this->Y + this->Height;
}

SDL_Rect Rectanglef::GetSDL_Rect()
{
    SDL_Rect rect;
    rect.x = (int)this->X;
    rect.y = (int)this->Y;
    rect.w = (int)this->Width;
    rect.h = (int)this->Height;
    return rect;
}