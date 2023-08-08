#pragma once

namespace Engine::Math
{
    class Vector3;
    class Vector3f
    {
        public:
        float X;
        float Y;
        float Z;

        Vector3f();
        Vector3f(float x, float y, float z);

        float Distance(Vector3f &other);
        void Normalize();
        float GetMagnitude();
        void SetMagnitude(float magnitude);
        Vector3 GetVector3();

        Vector3f operator+ (Vector3f const &other);
        Vector3f operator- (Vector3f const &other);
        Vector3f operator* (float mult);
        Vector3f operator/ (float mult);
    };
};