#pragma once

#include "Engine/Rendering/Sprite.h"
#include "Engine/Rendering/ImageTexture.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Math/Vector2f.h"
#include "Bullet.h"
#include "BossAttackData.h"

#include <vector>

#define BOSS_WIDTH 32
#define BOSS_HEIGHT 32

class Boss
{
    private:
        Engine::Math::Vector2f mPosition;
        Engine::Math::Vector2f mVelocity;

        int mDestroyed;
        int mHealth;
        int mBossType;
        float mTimer;
        float mMaxTime;
        int mCurrentAttack;
        int mCurrentIterator;
        int mStage;
        std::vector<BossAttackData> mAttackDatas;

    public:
        Boss();
        void Recycle(int bossType);
        
        void Draw(Engine::Rendering::Renderer &renderer);
        void Update(double deltaTime);
        std::vector<Bullet> SpawnBullets(Engine::Math::Vector2f playerPosition);
        Engine::Math::Vector2f GetPosition();
        Engine::Math::Rectangle GetRectangle();
        int GetMaxHealth();
        int GetHealth();
        void SetHealth(int health);
        int GetType();
        int IsDestroyed();
        void Destroy(int type);
        
};
