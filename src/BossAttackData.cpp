#include "BossAttackData.h"

#include "Engine/Helper/Format.h"
#include "Engine/Helper/Logger.h"

#include <vector>
#include <string>
#include <fstream>

BossAttackData::BossAttackData()
{
    this->mAttackType = AttackType::Straight;
    this->mStrength = 0;
    this->mTiming = 100;
    this->mBulletType = Bullet::Type::Static;
}

BossAttackData::BossAttackData(BossAttackData::AttackType attackType, int strength, float timing, Bullet::Type bulletType)
{
    this->mAttackType = attackType;
    this->mStrength = strength;
    this->mTiming = timing;
    this->mBulletType = bulletType;
}

BossAttackData::AttackType BossAttackData::GetAttackType()
{
    return this->mAttackType;
}

int BossAttackData::GetStrength()
{
    return this->mStrength;
}
float BossAttackData::GetTiming()
{
    return this->mTiming;
}

Bullet::Type BossAttackData::GetBulletType()
{
    return this->mBulletType;
}

void BossAttackData::SetAttackType(BossAttackData::AttackType attackType)
{
    this->mAttackType = attackType;
}

void BossAttackData::SetStrength(int strength)
{
    this->mStrength = strength;
}

void BossAttackData::SetTiming(float timing)
{
    this->mTiming = timing;
}

void BossAttackData::SetBulletType(Bullet::Type bulletType)
{
    this->mBulletType = bulletType;
}

void BossAttackData::WriteToFile(std::string path, std::string content)
{
    std::ofstream file(path);
    if (file.is_open())
    {
        file << content;
        file.close();
        Engine::Helper::Logger::Log(Engine::Helper::Logger::Info, Engine::Helper::string_format("Data written to file: %s", path.c_str()));
    }
    else
    {
        Engine::Helper::Logger::Log(Engine::Helper::Logger::Error, Engine::Helper::string_format("Failed to open file for writing: %s", path.c_str()));
    }
}

std::string BossAttackData::ReadFromFile(std::string path)
{
    std::string data;
    std::ifstream file(path);
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            data += line + "\n";
        }
        file.close();
    }
    else
    {
        Engine::Helper::Logger::Log(Engine::Helper::Logger::Error, Engine::Helper::string_format("Failed to open file for reading: %s", path.c_str()));
    }
    return data;
}

std::string BossAttackData::ToString(bool simple)
{
    if (simple)
    {
        std::string startString = Engine::Helper::string_format("%d;%d;%f;%d", static_cast<int>(this->mAttackType), this->mStrength, this->mTiming, this->mBulletType);
        return startString;
    }
    else
    {
        std::string startString = Engine::Helper::string_format("AttackType: %d; Strength: %d; Timing: %f; BulletType: %d", static_cast<int>(this->mAttackType), this->mStrength, this->mTiming, this->mBulletType);
        return startString;
    }
}

void BossAttackData::FromString(std::string data)
{
    std::vector<std::string> split = Engine::Helper::splitString(data, ';');
    this->mAttackType = static_cast<BossAttackData::AttackType>(std::stoi(split[0]));
    this->mStrength = std::stoi(split[1]);
    this->mTiming = std::stof(split[2]);
    this->mBulletType = static_cast<Bullet::Type>(std::stoi(split[3]));
}

void BossAttackData::WriteDatasToFile(std::string path, std::vector<BossAttackData> bossAttackData)
{
    std::string startString = Engine::Helper::string_format("%d", bossAttackData.size());
    for (int i = 0; i < bossAttackData.size(); i++)
    {
        startString += "|" + bossAttackData[i].ToString();
    }
    BossAttackData::WriteToFile(path, startString);
}

std::vector<BossAttackData> BossAttackData::LoadDatasFromFile(std::string path)
{
    std::string input = BossAttackData::ReadFromFile(path);
    std::vector<std::string> split = Engine::Helper::splitString(input, '|');
    int length = std::stoi(split[0]);
    std::vector<BossAttackData> datas;
    for (int i = 0; i < length; i++)
    {
        BossAttackData data;
        data.FromString(split[1+i]);
        datas.push_back(data);
    }
    return datas;
}