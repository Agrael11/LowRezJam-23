#include "Bullet.h"

#include "Engine/Math/MathHelper.h"
#include "SpriteManager.h"
#include <cmath>

using namespace Engine::Math;
using namespace Engine::Math::MathHelper;

Bullet::Bullet()
{
    this->mPosition = Vector2f(0,0);
    this->mVelocity = Vector2f(0,0);
    this->mPlayerOwned = false;
    this->mBulletType = Bullet::Type::Static;
    this->mDestroyed = false;
    this->mRotationAngle = 0;
}

Bullet::Bullet(Vector2f startingPosition, Vector2f startingVelocity, bool playerOwned, Bullet::Type type)
{
    this->mPosition = startingPosition;
    this->mVelocity = startingVelocity;
    this->mPlayerOwned = playerOwned;
    this->mBulletType = type;
    this->mDestroyed = false;
    this->mRotationAngle = 0;
}

void Bullet::Draw(Engine::Rendering::Renderer &renderer)
{
    if (!this->mDestroyed)
    {
        Rectangle dest = Rectangle((int)(this->mPosition.X + this->mOffset.X), (int)(this->mPosition.Y + this->mOffset.Y), BULLET_WIDTH, BULLET_HEIGHT);
        renderer.DrawSprite(SpriteManager::GetSprite((this->mBulletType == Bullet::Type::Static) ? "Bullets_0" : ((this->mBulletType == Bullet::Type::Zigzag) ? "Bullets_1" : "Bullets_2")), dest);
    }
}

void Bullet::Update(double deltaTime)
{
    if (!this->mDestroyed)
    {
        if (this->mBulletType == Bullet::Type::Zigzag)
        {
            this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f)) / 1.5f;
            this->mRotationAngle+=(float)deltaTime/100.f;
            if (this->mRotationAngle > TAUf)
            {
                this->mRotationAngle -= TAUf;
            }
            float heading = this->mVelocity.GetHeading() + HALF_PIf;
            this->mOffset.X = 0;
            this->mOffset.Y = 1;
            this->mOffset.SetHeading(heading);
            this->mOffset.SetMagnitude(std::sin(this->mRotationAngle)*4);
        }
        else if (this->mBulletType == Bullet::Type::Circling)
        {
            this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f)) / 4;
            this->mRotationAngle+=(float)deltaTime/100.f;
            if (this->mRotationAngle > TAUf)
            {
                this->mRotationAngle -= TAUf;
            }
            float heading = this->mVelocity.GetHeading() + HALF_PIf;
            this->mOffset.X = std::cos(this->mRotationAngle)*3;
            this->mOffset.Y = std::sin(this->mRotationAngle)*-3;
        }
        else
        {
            this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f));
            this->mOffset.X = 0;
            this->mOffset.Y = 0;
        }
        
        if (this->mPosition.X < -8 || this->mPosition.X > 72 ||
        this->mPosition.Y < -8 || this->mPosition.Y > 72)
        {
            this->Destroy();
        }
    }
}

Engine::Math::Vector2f Bullet::GetPosition()
{
    return this->mPosition + this->mOffset;
}

Rectangle Bullet::GetRectangle()
{
    return Rectangle((int)this->mPosition.X, (int)this->mPosition.Y, BULLET_WIDTH, BULLET_HEIGHT);
}

Bullet::Type Bullet::GetBulletType()
{
    return this->mBulletType;
}

bool Bullet::IsDestroyed()
{
    return this->mDestroyed;
}

void Bullet::Destroy()
{
    this->mDestroyed = true;
}

bool Bullet::IsOwnedByPlayer()
{
    return this->mPlayerOwned;
}