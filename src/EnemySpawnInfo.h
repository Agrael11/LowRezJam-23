#pragma once

#include "Engine/Math/Vector2f.h"
#include "Bullet.h"
#include <string>

class EnemySpawnInfo
{
    private:
        Bullet::Type mBulletType;
        float mStartTimer;
        Engine::Math::Vector2f mStartPosition;
        Engine::Math::Vector2f mDirection;
        
    public:
        EnemySpawnInfo() {}
        EnemySpawnInfo(Bullet::Type bulletType, float startTimer, Engine::Math::Vector2f startPosition, Engine::Math::Vector2f direction);

        Bullet::Type GetBulletType();
        float GetStartTimer();
        Engine::Math::Vector2f GetStartPosition();
        Engine::Math::Vector2f GetDirection();

        void SetBulletType(Bullet::Type bulletType);
        void SetStartTimer(float startTimer);
        void SetStartPosition(Engine::Math::Vector2f startPosition);
        void SetDirection(Engine::Math::Vector2f direction);

        std::string ToString(bool simple=true);
        void FromString(std::string data);
};