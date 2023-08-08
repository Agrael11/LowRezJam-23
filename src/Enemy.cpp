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
}

void Enemy::Draw(Engine::Rendering::Renderer &renderer)
{
    if (this->mDestroyed == 0)
    {
        Rectangle dest = Rectangle((int)this->mPosition.X, (int)this->mPosition.Y, ENEMY_WIDTH, ENEMY_HEIGHT);
        renderer.DrawSprite(SpriteManager::GetSprite(this->mBulletType == Bullet::Type::Static ? "Enemy_Straight" : (this->mBulletType == Bullet::Type::Zigzag ? "Enemy_Zigzag" : "Enemy_Circle")) , dest);
    }
}

void Enemy::Update(double deltaTime)
{
    if (this->mDestroyed == 0)
    {
        this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f) * 0.5f);

        if (this->mPosition.X < -64 || this->mPosition.X > 128 ||
        this->mPosition.Y < -64 || this->mPosition.Y > 128) this->Destroy(2);

        this->mTimer += ((float)deltaTime/17.f)*2.f;
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
}