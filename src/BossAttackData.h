#pragma once

#include "Bullet.h"
#include <vector>

class BossAttackData
{
    public:
        enum class AttackType {Circle, Cone, Straight, Wait};
    
    private:
        AttackType mAttackType;
        int mStrength;
        float mTiming;
        Bullet::Type mBulletType;
    
    public:
        BossAttackData();
        BossAttackData(AttackType attackType, int strength, float timing, Bullet::Type bulletType);

        AttackType GetAttackType();
        int GetStrength();
        float GetTiming();
        Bullet::Type GetBulletType();

        void SetAttackType(AttackType attackType);
        void SetStrength(int strength);
        void SetTiming(float timing);
        void SetBulletType(Bullet::Type bulletType);

        std::string ToString(bool simple=true);
        void FromString(std::string data);
        static void WriteToFile(std::string path, std::string content);
        static std::string ReadFromFile(std::string path);
        static void WriteDatasToFile(std::string path, std::vector<BossAttackData> bossAttackData);
        static std::vector<BossAttackData> LoadDatasFromFile(std::string path);
};