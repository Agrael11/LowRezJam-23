#pragma once

namespace Engine::Math
{
    class Vector3f;
    class Vector3
    {
        public:
        int X;
        int Y;
        int Z;

        Vector3();
        Vector3(int x, int y, int z);

        float Distance(Vector3 &other);
        void Normalize();
        float GetMagnitude();
        void SetMagnitude(float magnitude);
        Vector3f GetVector3f();

        Vector3 operator+ (Vector3 const &other);
        Vector3 operator- (Vector3 const &other);
        Vector3 operator* (float mult);
        Vector3 operator/ (float mult);
    };
};