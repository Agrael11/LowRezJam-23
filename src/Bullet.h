#pragma once

#include "Engine/Rendering/Sprite.h"
#include "Engine/Rendering/ImageTexture.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Math/Vector2f.h"

#define BULLET_WIDTH 2
#define BULLET_HEIGHT 2

class Bullet
{
    public: enum class Type {Static, Zigzag, Circling};
    private:
        Engine::Math::Vector2f mPosition;
        Engine::Math::Vector2f mOffset;
        Engine::Math::Vector2f mVelocity;
        float mRotationAngle;
        Type mBulletType;
        bool mDestroyed;
        bool mPlayerOwned;
        Bullet();

    public:

        Bullet(Engine::Math::Vector2f startingPosition, Engine::Math::Vector2f startingVelocity, bool playerOwned = false, Type type = Type::Static);

        void Draw(Engine::Rendering::Renderer &renderer);
        void Update(double deltaTime);
        Engine::Math::Vector2f GetPosition();
        Engine::Math::Rectangle GetRectangle();
        Type GetBulletType();
        bool IsDestroyed();
        void Destroy();
        bool IsOwnedByPlayer();
};
