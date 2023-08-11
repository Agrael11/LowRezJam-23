#include "EnemySpawnInfo.h"
#include "Engine/Helper/Format.h"


EnemySpawnInfo::EnemySpawnInfo(Bullet::Type bulletType, float startTimer, Engine::Math::Vector2f startPosition, Engine::Math::Vector2f direction)
{
    this->mBulletType = bulletType,
    this->mStartTimer = startTimer;
    this->mStartPosition = startPosition;
    this->mDirection = direction;
}

Bullet::Type EnemySpawnInfo::GetBulletType()
{
    return this->mBulletType;
}

float EnemySpawnInfo::GetStartTimer()
{
    return this->mStartTimer;
}

Engine::Math::Vector2f EnemySpawnInfo::GetStartPosition()
{
    return this->mStartPosition;
}

Engine::Math::Vector2f EnemySpawnInfo::GetDirection()
{
    return this->mDirection;
}

void EnemySpawnInfo::SetBulletType(Bullet::Type bulletType)
{
    this->mBulletType = bulletType;
}

void EnemySpawnInfo::SetStartTimer(float startTimer)
{
    this->mStartTimer = startTimer;
}

void EnemySpawnInfo::SetStartPosition(Engine::Math::Vector2f startPosition)
{
    this->mStartPosition = startPosition;
}

void EnemySpawnInfo::SetDirection(Engine::Math::Vector2f direction)
{
    this->mDirection = direction;
}

std::string EnemySpawnInfo::ToString(bool simple)
{
    if (simple)
    {
        return Engine::Helper::string_format("%d;%f;%f:%f;%f:%f",
        static_cast<int>(this->mBulletType), this->mStartTimer, this->mStartPosition.X, this->mStartPosition.Y, this->mDirection.X, this->mDirection.Y);
    }
    else
    {
        return Engine::Helper::string_format("BulletType: %d; StartTimer: %.2f; StartPosition %.2fx%.2f; Direction: %.2fx%.2f",
        static_cast<int>(this->mBulletType), this->mStartTimer, this->mStartPosition.X, this->mStartPosition.Y, this->mDirection.X, this->mDirection.Y);
    }
}

void EnemySpawnInfo::FromString(std::string data)
{
    std::vector<std::string> split = Engine::Helper::splitString(data, ';');
    this->mBulletType = static_cast<Bullet::Type>(std::stoi(split[0]));
    this->mStartTimer = std::stof(split[1]);
    std::vector<std::string> split2 = Engine::Helper::splitString(split[2], ':');
    this->mStartPosition = Engine::Math::Vector2f(std::stof(split2[0]),std::stof(split2[1]));
    split2 = Engine::Helper::splitString(split[3], ':');
    this->mDirection = Engine::Math::Vector2f(std::stof(split2[0]),std::stof(split2[1]));
}