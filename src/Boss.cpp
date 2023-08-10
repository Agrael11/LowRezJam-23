#include "Boss.h"

#include "Engine/Math/Rectangle.h"
#include "Engine/Rendering/ImageTexture.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Engine/Rendering/Sprite.h"
#include "Engine/Helper/Format.h"
#include "Engine/Helper/Logger.h"
#include "Engine/Math/MathHelper.h"
#include <cmath>

using namespace Engine::Math;
using namespace Engine::Math::MathHelper;

Boss::Boss()
{
}

void Boss::Recycle(int bossType)
{
    this->mPosition = Vector2f(16.f, 128.f);
    this->mVelocity = Vector2f(0, 1.f);
    this->mHealth = 100;
    this->mBossType = bossType;
    this->mDestroyed = 0;
    this->mTimer = 0;
    this->mMaxTime = 100;
    this->mStage = 0;
    this->mAttackDatas = BossAttackData::LoadDatasFromFile("Assets/BossAttackDatas_0.bad");
    this->mCurrentAttack = 0;
    this->mCurrentIterator = 0;
}

void Boss::Draw(Engine::Rendering::Renderer &renderer)
{
    if (this->mDestroyed == 0)
    {
        Rectangle dest = Rectangle((int)this->mPosition.X, (int)this->mPosition.Y, BOSS_WEIGHT, BOSS_HEIGHT);
        renderer.DrawSprite(SpriteManager::GetSprite(Engine::Helper::string_format("Boss_%d",mBossType)) , dest);
    }
}

void Boss::Update(double deltaTime)
{
    if (this->mDestroyed == 0)
    {
        switch (this->mStage)
        {
            case 0:
                this->mPosition = this->mPosition - (this->mVelocity * ((float)deltaTime/17.f) * 0.5f);
                if (this->mPosition.Y < 37)
                {
                    this->mStage = 1;
                }
                break;
            case 1:
                if (this->mAttackDatas[this->mCurrentAttack].GetAttackType() == BossAttackData::AttackType::Cone)
                {
                    this->mPosition = this->mPosition - (this->mVelocity * ((float)deltaTime/17.f) * 0.f);
                    if (this->mPosition.Y > 58)
                    {
                        this->mPosition = this->mPosition - (this->mVelocity * ((float)deltaTime/17.f) * 0.5f);
                    }
                    this->mTimer += ((float)deltaTime/17.f)*2.f;
                    if (this->mPosition.Y < 56)
                    {
                        this->mStage = 2;
                    }
                }
                else if (this->mAttackDatas[this->mCurrentAttack].GetAttackType() == BossAttackData::AttackType::Straight)
                {
                    this->mPosition.X = this->mPosition.X - (this->mVelocity.Y * ((float)deltaTime/17.f) * 0.5f);
                    this->mTimer += ((float)deltaTime/17.f)*2.f;
                    if (this->mPosition.X < -8)
                    {
                        this->mStage = 2;
                    }
                }
                else
                {
                    if (this->mPosition.X > 16)
                    {
                        this->mPosition.X = this->mPosition.X - (this->mVelocity.Y * ((float)deltaTime/17.f) * 0.5f);
                    }
                    else if (this->mPosition.X > 15)
                    {
                        this->mPosition.X = this->mPosition.X - (this->mVelocity.Y * ((float)deltaTime/17.f) * 0.5f);
                    }
                    else if (this->mPosition.X < 14)
                    {
                        this->mPosition.X = this->mPosition.X + (this->mVelocity.Y * ((float)deltaTime/17.f) * 0.5f);
                    }
                    else if (this->mPosition.X < 15)
                    {
                        this->mPosition.X = this->mPosition.X + (this->mVelocity.Y * ((float)deltaTime/17.f) * 0.5f);
                    }
                    if (this->mPosition.Y > 45)
                    {
                        this->mStage = 0;
                    }
                    this->mPosition = this->mPosition - (this->mVelocity * ((float)deltaTime/17.f) * 0.1f);
                    this->mTimer += ((float)deltaTime/17.f)*2.f;
                    if (this->mPosition.Y < 37)
                    {
                        this->mStage = 2;
                    }
                }
                break;
            case 2:
                if (this->mAttackDatas[this->mCurrentAttack].GetAttackType() == BossAttackData::AttackType::Cone)
                {
                    this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f) * 0.f);
                    if (this->mPosition.Y < 62)
                    {
                        this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f) * 0.5f);
                    }
                    this->mTimer += ((float)deltaTime/17.f)*2.f;
                    if (this->mPosition.Y > 56)
                    {
                        this->mStage = 1;
                    }
                }
                else if (this->mAttackDatas[this->mCurrentAttack].GetAttackType() == BossAttackData::AttackType::Straight)
                {
                    this->mPosition.X = this->mPosition.X + (this->mVelocity.Y * ((float)deltaTime/17.f) * 0.5f);
                    this->mTimer += ((float)deltaTime/17.f)*2.f;
                    if (this->mPosition.X > 40)
                    {
                        this->mStage = 1;
                    }
                }
                else
                {
                    if (this->mPosition.X > 16)
                    {
                        this->mPosition.X = this->mPosition.X - (this->mVelocity.Y * ((float)deltaTime/17.f) * 0.5f);
                    }
                    else if (this->mPosition.X < 14)
                    {
                        this->mPosition.X = this->mPosition.X + (this->mVelocity.Y * ((float)deltaTime/17.f) * 0.5f);
                    }
                    else if (this->mPosition.X < 15 || this->mPosition.X > 15)
                    {
                        this->mPosition.X = 15;
                    }
                    this->mPosition = this->mPosition + (this->mVelocity * ((float)deltaTime/17.f) * 0.1f);
                    this->mTimer += ((float)deltaTime/17.f)*2.f;
                    if (this->mPosition.Y > 43)
                    {
                        this->mStage = 1;
                    }
                }
                break;
        }
    }
}

Vector2f Boss::GetPosition()
{
    return this->mPosition;
}

std::vector<Bullet> Boss::SpawnBullets()
{
    std::vector<Bullet> bullets;
    if (this->mStage == 1 || this->mStage == 2)
    {
        BossAttackData currentAttack = this->mAttackDatas[this->mCurrentAttack];
        if(currentAttack.GetAttackType() == BossAttackData::AttackType::Straight && this->mTimer > currentAttack.GetTiming()/currentAttack.GetStrength()*this->mCurrentIterator)
        {
            this->mCurrentIterator++;
            Vector2f position = Vector2f(this->mPosition.X, this->mPosition.Y);
            position.X += 15;
            bullets.push_back(Bullet(position, Vector2f(0,-1), false, currentAttack.GetBulletType()));
            Engine::Helper::Logger::Log(Engine::Helper::Logger::Info, Engine::Helper::string_format("Now attacking in pattern: %s", this->mAttackDatas[this->mCurrentAttack].ToString(false).c_str()));
        }
        else if (currentAttack.GetAttackType() == BossAttackData::AttackType::Circle && this->mTimer > currentAttack.GetTiming()/2.f && this->mCurrentIterator == 0)
        {
            this->mCurrentIterator++;
            Vector2f position = Vector2f(this->mPosition.X, this->mPosition.Y);
            position.X += 15;
            position.Y += 16;
            for (float i = 0; i < TAUf; i+=TAUf/currentAttack.GetStrength())
            {
                float x = sin(i);
                float y = cos(i);
                bullets.push_back(Bullet(position, Vector2f(x, y), false, currentAttack.GetBulletType()));
            }
            bullets.push_back(Bullet(position, Vector2f(0,-1), false, currentAttack.GetBulletType()));
            Engine::Helper::Logger::Log(Engine::Helper::Logger::Info, Engine::Helper::string_format("Now attacking in pattern: %s", this->mAttackDatas[this->mCurrentAttack].ToString(false).c_str()));
        }
        else if (currentAttack.GetAttackType() == BossAttackData::AttackType::Cone && this->mTimer > currentAttack.GetTiming()/2.f && this->mCurrentIterator == 0)
        {
            this->mCurrentIterator++;
            Vector2f position = Vector2f(this->mPosition.X, this->mPosition.Y);
            position.X += 15;
            for (float i = -30; i <= 35; i+=60.f/(currentAttack.GetStrength()-1))
            {
                bullets.push_back(Bullet(position, Vector2f(i/30.f,-0.5f), false, currentAttack.GetBulletType()));
            }
            Engine::Helper::Logger::Log(Engine::Helper::Logger::Info, Engine::Helper::string_format("Now attacking in pattern: %s", this->mAttackDatas[this->mCurrentAttack].ToString(false).c_str()));
        }
        
        if(this->mTimer > this->mAttackDatas[this->mCurrentAttack].GetTiming())
        {
            this->mCurrentIterator = 0;
            this->mTimer = 0;
            this->mCurrentAttack++;
            if (this->mCurrentAttack >= this->mAttackDatas.size())
            {
                this->mCurrentAttack = 0;
            }
        }
        /*else
        {
            Engine::Helper::Logger::Log(Engine::Helper::Logger::Info, Engine::Helper::string_format("Wating: %0.4f/%0.4f", this->mTimer, this->mAttackDatas[this->mCurrentAttack].GetTiming()));
        }*/
    }
    return bullets;
}

Rectangle Boss::GetRectangle()
{
    return Rectangle((int)this->mPosition.X, (int)this->mPosition.Y, BOSS_WEIGHT, BOSS_HEIGHT);
}

int Boss::GetHealth()
{
    return this->mHealth;
}

void Boss::SetHealth(int health)
{
    this->mHealth = health;
    if (this->mHealth > 100) this->mHealth = 100;
    if (this->mHealth < 0) this->mHealth = 0;
}

int Boss::GetType()
{
    return this->mBossType;
}

int Boss::IsDestroyed()
{
    return this->mDestroyed;
}

void Boss::Destroy(int type)
{
    this->mDestroyed = type;
}