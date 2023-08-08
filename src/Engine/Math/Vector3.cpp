#include "Vector3.h"

#include <cmath>

#include "Vector3f.h"

using namespace Engine::Math;

Vector3::Vector3()
{
    this->X = 0;
    this->Y = 0;
    this->Z = 0;
}

Vector3::Vector3(int x, int y, int z)
{
    this->X = x;
    this->Y = y;
    this->Z = z;
}

float Vector3::Distance(Vector3 &other)
{
    int a = abs(this->X - other.X);
    int b = abs(this->Y - other.Y);
    int c = abs(this->Y - other.Y);
    return sqrt((float)(a * a + b * b + c * c));
}

void Vector3::Normalize()
{
    float m = GetMagnitude();;
    if (m > 0)
    {
        this->X = (int)(this->X / m);
        this->Y = (int)(this->Y / m);
    }
}

float Vector3::GetMagnitude()
{
    return sqrt((float)(this->X * this->X + this->Y * this->Y + this->Z * this->Z));
}

void Vector3::SetMagnitude(float magnitude)
{
    this->Normalize();
    this->X = (int)(this->X * magnitude);
    this->Y = (int)(this->Y * magnitude);
    this->Z = (int)(this->Z * magnitude);
}

Vector3f Vector3::GetVector3f()
{
    return Vector3f((float)this->X, (float)this->Y, (float)this->Z);
}

Vector3 Vector3::operator+ (Vector3 const &other)
{
    return Vector3(this->X + other.X, this->Y + other.Y, this->Z + other.Z);
}

Vector3 Vector3::operator- (Vector3 const &other)
{
    return Vector3(this->X - other.X, this->Y - other.Y, this->Z - other.Z);
}

Vector3 Vector3::operator* (float mult)
{
    return Vector3((int)(this->X * mult), (int)(this->Y * mult), (int)(this->Z * mult));
}

Vector3 Vector3::operator/ (float mult)
{
    return Vector3((int)(this->X / mult), (int)(this->Y / mult), (int)(this->Z / mult));
}