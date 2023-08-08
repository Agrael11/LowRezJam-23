#pragma once

#include "Engine/Rendering/Sprite.h"
#include "Engine/Rendering/ImageTexture.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Math/Vector2f.h"
#include "Bullet.h"
#include "EnemySpawnInfo.h"

#define ENEMY_WIDTH 8
#define ENEMY_HEIGHT 4

class Enemy
{
    private:
        Engine::Math::Vector2f mPosition;
        Engine::Math::Vector2f mVelocity;

        int mDestroyed;
        int mHealth;
        Bullet::Type mBulletType;
        float mTimer;
        float mMaxTime;

    public:
        Enemy(Bullet::Type type, Engine::Math::Vector2f startPosition, Engine::Math::Vector2f velocity);
        Enemy(EnemySpawnInfo spawnInfo);

        void Draw(Engine::Rendering::Renderer &renderer);
        void Update(double deltaTime);
        bool TimerCheck();
        Bullet SpawnBullet();
        Engine::Math::Vector2f GetPosition();
        Engine::Math::Rectangle GetRectangle();
        int GetHealth();
        void SetHealth(int health);
        Bullet::Type GetType();
        int IsDestroyed();
        void Destroy(int type);
};
