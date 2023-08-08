#include "Rectangle.h"

#include <SDL.h>

using namespace Engine::Math;

Rectangle::Rectangle()
{
    this->X = 0;
    this->Y = 0;
    this->Width = 0;
    this->Height = 0;
}

Rectangle::Rectangle(int x, int y, int width, int height)
{
    this->X = x;
    this->Y = y;
    this->Width = width;
    this->Height = height;
}

bool Rectangle::Intersects(Rectangle &other)
{
    return (
        this->GetX2() > other.X &&
        this->X < other.GetX2() &&
        this->GetY2() > other.Y &&
        this->Y < other.GetY2());
}

bool Rectangle::Contains(Rectangle &other)
{
    return (
        this->X > other.X &&
        this->GetX2() < other.GetX2() &&
        this->Y > other.Y &&
        this->GetY2() < other.GetY2());
}

int Rectangle::GetX2()
{
    return this->X + this->Width;
}

int Rectangle::GetY2()
{
    return this->Y + this->Height;
}

SDL_Rect Rectangle::GetSDL_Rect()
{
    SDL_Rect rect;
    rect.x = this->X;
    rect.y = this->Y;
    rect.w = this->Width;
    rect.h = this->Height;
    return rect;
}