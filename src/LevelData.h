#pragma once

#include "EnemySpawnInfo.h"
#include <vector>

class LevelData
{
    private:
        std::vector<EnemySpawnInfo> mEnemySpawnInfos;
        int mBackground;
        int mBossId;
        //maybe later bgtile setup?

    public:
        LevelData() {}
        LevelData(std::vector<EnemySpawnInfo> enemySpawnInfo, int background = 0, int bossId = 0);

        float GetLevelLength();

        EnemySpawnInfo GetEnemySpawnInfo(int id);
        std::vector<EnemySpawnInfo>& GetEnemySpawnInfos();
        void SetEnemySpawnInfos(std::vector<EnemySpawnInfo> infos);
        void AddEnemySpawnInfo(EnemySpawnInfo info);
        int GetBackground();
        void SetBackground(int background);
        int GetBossId();
        void SetBossId(int bossId);
        std::string ToString(bool simple=true);
        void FromString(std::string data);
};