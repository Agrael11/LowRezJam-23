#pragma once

#include "Engine/Helper/Logger.h"
#include "Engine/Helper/Format.h"
#include "LevelData.h"
#include "EnemySpawnInfo.h"
#include <vector>
#include <string>
#include <fstream>

std::vector<LevelData> levels;

void LoadLevelFromFile(std::string path)
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
    LevelData level;
    level.FromString(data);
    levels.push_back(level);
}