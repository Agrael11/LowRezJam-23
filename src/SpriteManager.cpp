#include "SpriteManager.h"
#include "Engine/Helper/Format.h"
#include "Engine/Helper/Logger.h"
#include <unordered_map>

using namespace std;
using namespace Engine::Rendering;
using namespace Engine::Helper;

static std::unordered_map<std::string, Engine::Rendering::Sprite> mSprites;

SpriteManager::SpriteManager()
{
    
}

bool SpriteManager::AddSprite(string id, Sprite sprite)
{
    if (SpriteManager::SpriteExists(id))
    {
        Logger::Log(Logger::Error, string_format("Texture %s already loaded", id.c_str()));
        return false;
    }

    mSprites[id] = sprite;

    Logger::Log(Logger::Info, string_format("Added %s to Sprite Manager", id.c_str()));
    return true;
}

bool SpriteManager::SpriteExists(string id)
{
    return (mSprites.count(id) > 0);
}

Sprite& SpriteManager::GetSprite(string id)
{
    if (!SpriteManager::SpriteExists(id))
    {
        Logger::Log(Logger::Error, string_format("Sprite %s doesn't exist", id.c_str()));
        throw std::runtime_error(string_format("Sprite %s doesn't exist", id.c_str()));
    }

    return mSprites[id];
}

bool SpriteManager::RemoveSprite(string id)
{
    if (!SpriteManager::SpriteExists(id))
    {
        Logger::Log(Logger::Error, string_format("Sprite %s doesn't exist", id.c_str()));
        return false;
    }
    
    Sprite texture = GetSprite(id);
    texture.Unload();

    mSprites.erase(id);
    return true;
}