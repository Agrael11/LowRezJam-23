#include "LevelData.h"
#include "Engine/Helper/Format.h"

#include <vector>

LevelData::LevelData(std::vector<EnemySpawnInfo> enemySpawnInfo, int background, int bossId)
{
    this->mEnemySpawnInfos = enemySpawnInfo;
    this->mBackground = background;
    this->mBossId = bossId;
}

float LevelData::GetLevelLength()
{
    float timer = 0;
    for (auto& spawnInfo : this->mEnemySpawnInfos)
    {
        timer += spawnInfo.GetStartTimer();
    }
    return timer;
}

EnemySpawnInfo LevelData::GetEnemySpawnInfo(int id)
{
    return this->mEnemySpawnInfos[id];
}

std::vector<EnemySpawnInfo>& LevelData::GetEnemySpawnInfos()
{
    return this->mEnemySpawnInfos;
}

void LevelData::SetEnemySpawnInfos(std::vector<EnemySpawnInfo> infos)
{
    this->mEnemySpawnInfos = infos;
}

void LevelData::AddEnemySpawnInfo(EnemySpawnInfo info)
{
    this->mEnemySpawnInfos.push_back(info);
}

int LevelData::GetBackground()
{
    return this->mBackground;
}

void LevelData::SetBackground(int background)
{
    this->mBackground = background;
}

int LevelData::GetBossId()
{
    return this->mBossId;
}

void LevelData::SetBossId(int bossId)
{
    this->mBossId = bossId;
}

std::string LevelData::ToString(bool simple)
{
    if (simple)
    {
        std::string startString = Engine::Helper::string_format("%d|%d|%d", this->mBackground, this->mBossId, this->mEnemySpawnInfos.size());
        for (int i = 0; i < this->mEnemySpawnInfos.size(); i++)
        {
            startString += "|" + this->mEnemySpawnInfos[i].ToString();
        }
        return startString;
    }
    else
    {
        std::string startString = Engine::Helper::string_format("Background: %d; BossId: %d; EnemyCount: %d; Enemies:", this->mBackground, this->mBossId, this->mEnemySpawnInfos.size());
        for (int i = 0; i < this->mEnemySpawnInfos.size(); i++)
        {
            startString += "\n" + this->mEnemySpawnInfos[i].ToString(false);
        }
        return startString;
    }
}

void LevelData::FromString(std::string data)
{
    std::vector<std::string> split = Engine::Helper::splitString(data, '|');
    this->mBackground = std::stoi(split[0]);
    this->mBossId = std::stoi(split[1]);
    int length = std::stoi(split[2]);
    for (int i = 0; i < length; i++)
    {
        EnemySpawnInfo info;
        info.FromString(split[3+i]);
        this->mEnemySpawnInfos.push_back(info);
    }
}