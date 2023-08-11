#include "Enemy.h"

#include "Engine/Math/Rectangle.h"
#include "Engine/Rendering/ImageTexture.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Engine/Rendering/Sprite.h"

using namespace Engine::Math;

Enemy::Enemy(Bullet::Type type, Vector2f startPosition, Vector2f velocity)
{
    this->mPosition = startPosition;
    this->mVelocity = velocity;
    this->mHealth = 100;
    this->mBulletType = type;
    this->mDestroyed = 0;
    this->mTimer = 0;
    this->mMaxTime = 100;
    this->mAnimating = 0;
}

Enemy::Enemy(EnemySpawnInfo spawnInfo)
{
    this->mPosition = spawnInfo.GetStartPosition();
    this->mVelocity = spawnInfo.GetDirection();
    this->mHealth = 100;
    this->mBulletType = spawnInfo.GetBulletType();
    this->mDestroyed = 0;
    this->mTimer = 0;
    this->mMaxTime = 100;
    this->mAnimating = 0;
}

void Enemy::Draw(Engine::Rendering::Renderer &renderer)
{
    Engine::Rendering::Sprite sprite= Engine::Rendering::Sprite(SpriteManager::GetSprite(this->mBulletType == Bullet::Type::Static ? "Enemy_Straight" : (this->mBulletType == Bullet::Type::Zigzag ? "Enemy_Zigzag" : "Enemy_Circle")));
    if (this->mDestroyed == 0 || this->mDestroyed == 3)
    {
        Colorf color(1.f, 1.f, 1.f, 1.f);
        sprite.SetColorMod(color);
        Rectangle dest = Rectangle((int)this->mPosition.X, (int)this->mPosition.Y, ENEMY_WIDTH, ENEMY_HEIGHT);
        renderer.DrawSprite(sprite , dest);
    }
    else if (this->mAnimating > 0 && this->mDestroyed == 1)
    {
        Rectangle dest = Rectangle((int)this->mPosition.X, (int)this->mPosition.Y, ENEMY_WIDTH, ENEMY_HEIGHT);
        Colorf color(1.f, 1.f, 1.f, (float)(this->mAnimating/30.));
        sprite.SetColorMod(color);
        renderer.DrawSprite(sprite , dest);
        color.R = 1.f;
        color.G = 1.f;
        color.B = 1.f;
        color.A = 1.f;
        sprite.SetColorMod(color);
    }
}

void Enemy::Update(double deltaTime)
{
    if (this->mDestroyed == 0)
    {
        this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f) * 0.5f);

        bool outOfBounds = false;
        outOfBounds |= this->mVelocity.X < 0 && this->mPosition.X < -ENEMY_WIDTH;
        outOfBounds |= this->mVelocity.X > 0 && this->mPosition.X > 64+ENEMY_WIDTH;
        outOfBounds |= this->mVelocity.Y < 0 && this->mPosition.Y < -ENEMY_HEIGHT;
        outOfBounds |= this->mVelocity.Y > 0 && this->mPosition.Y > 64+ENEMY_HEIGHT;
        if (outOfBounds) this->Destroy(2);

        this->mTimer += ((float)deltaTime/17.f)*2.f;
    }
    else if (this->mAnimating > 0)
    {
        this->mAnimating -= deltaTime/4.f;
        this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f) * 0.5f);
    }
    else if (this->mDestroyed == 3)
    {
        this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f) * 0.5f);
        bool outOfBounds = false;
        outOfBounds |= this->mVelocity.X < 0 && this->mPosition.X < -ENEMY_WIDTH;
        outOfBounds |= this->mVelocity.X > 0 && this->mPosition.X > 64+ENEMY_WIDTH;
        outOfBounds |= this->mVelocity.Y < 0 && this->mPosition.Y < -ENEMY_HEIGHT;
        outOfBounds |= this->mVelocity.Y > 0 && this->mPosition.Y > 64+ENEMY_HEIGHT;
        if (outOfBounds) this->Destroy(4);
    }
}

Vector2f Enemy::GetPosition()
{
    return this->mPosition;
}

bool Enemy::TimerCheck()
{
    if (this->mTimer > this->mMaxTime)
    {
        this->mTimer = 0;
        return true;
    }
    return false;
}

Bullet Enemy::SpawnBullet()
{
    Vector2f position = Vector2f(this->mPosition.X, this->mPosition.Y);
    position.X += 2;
    return Bullet(position, Vector2f(0,-1), false, this->mBulletType);
}

Rectangle Enemy::GetRectangle()
{
    return Rectangle((int)this->mPosition.X, (int)this->mPosition.Y, ENEMY_WIDTH, ENEMY_HEIGHT);
}

int Enemy::GetHealth()
{
    return this->mHealth;
}

void Enemy::SetHealth(int health)
{
    this->mHealth = health;
    if (this->mHealth > 100) this->mHealth = 100;
    if (this->mHealth < 0) this->mHealth = 0;
}

Bullet::Type Enemy::GetType()
{
    return this->mBulletType;
}

int Enemy::IsDestroyed()
{
    return this->mDestroyed;
}

void Enemy::Destroy(int type)
{
    this->mDestroyed = type;
    this->mAnimating = 30;
}

double Enemy::GetAnimating()
{
    return this->mAnimating;
}