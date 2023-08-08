#include "Colorf.h"
#include "Color.h"

using namespace Engine::Math;

Colorf::Colorf()
{
    this->R = 0;
    this->G = 0;
    this->B = 0;
    this->A = 0;
}

Colorf::Colorf(float r, float g, float b, float a)
{
    this->R = r;
    this->G = g;
    this->B = b;
    this->A = a;
}

Color Colorf::GetColor()
{
    return Color((int)(this->R*255),(int)(this->G*255),(int)(this->B*255),(int)(this->A*255));
}