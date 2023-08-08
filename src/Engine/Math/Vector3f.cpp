#include "Vector3f.h"

#include <cmath>

#include "Vector3.h"

using namespace Engine::Math;

Vector3f::Vector3f()
{
    this->X = 0;
    this->Y = 0;
    this->Z = 0;
}

Vector3f::Vector3f(float x, float y, float z)
{
    this->X = x;
    this->Y = y;
    this->Z = z;
}

float Vector3f::Distance(Vector3f &other)
{
    float a = abs(this->X - other.X);
    float b = abs(this->Y - other.Y);
    float c = abs(this->Z - other.Z);
    return sqrt(a * a + b * b + c *c);
}

void Vector3f::Normalize()
{
    float m = GetMagnitude();;
    if (m > 0)
    {
        this->X /= m;
        this->Y /= m;
        this->Z /= m;
    }
}

float Vector3f::GetMagnitude()
{
    return sqrt(this->X * this->X + this->Y * this->Y + this->Z * this->Z);
}

void Vector3f::SetMagnitude(float magnitude)
{
    this->Normalize();
    this->X *= magnitude;
    this->Y *= magnitude;
    this->Z *= magnitude;
}

Vector3 Vector3f::GetVector3()
{
    return Vector3((int)this->X, (int)this->Y, (int)this->Z);
}

Vector3f Vector3f::operator+ (Vector3f const &other)
{
    return Vector3f(this->X + other.X, this->Y + other.Y, this->Z + other.Z);
}

Vector3f Vector3f::operator- (Vector3f const &other)
{
    return Vector3f(this->X - other.X, this->Y - other.Y, this->Z - other.Z);
}

Vector3f Vector3f::operator* (float mult)
{
    return Vector3f(this->X * mult, this->Y * mult, this->Z * mult);
}

Vector3f Vector3f::operator/ (float mult)
{
    return Vector3f(this->X / mult, this->Y / mult, this->Z / mult);
}