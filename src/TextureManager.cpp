#include "TextureManager.h"
#include "Engine/Helper/Format.h"
#include "Engine/Helper/Logger.h"

using namespace std;
using namespace Engine::Rendering;
using namespace Engine::Helper;

static std::unordered_map<std::string, Engine::Rendering::ImageTexture> mTextures;

TextureManager::TextureManager()
{
    
}

bool TextureManager::LoadTexture(string id, string filename, Renderer &renderer)
{
    if (TextureManager::TextureExists(id))
    {
        Logger::Log(Logger::Error, string_format("Texture %s already loaded", id.c_str()));
        return false;
    }
    
    ImageTexture texture;
    if (!texture.Load(filename, renderer))
    {
        return false;
    }

    mTextures[id] = texture;
    Logger::Log(Logger::Info, string_format("Added %s to Texture Manager", id.c_str()));
    
    return true;
}

bool TextureManager::TextureExists(string id)
{
    return (mTextures.count(id) > 0);
}

ImageTexture& TextureManager::GetTexture(string id)
{
    if (!TextureManager::TextureExists(id))
    {
        Logger::Log(Logger::Error, string_format("Texture %s doesn't exist", id.c_str()));
        throw std::runtime_error(string_format("Texture %s doesn't exist", id.c_str()));
    }

    return mTextures[id];
}

bool TextureManager::RemoveTexture(string id)
{
    if (!TextureManager::TextureExists(id))
    {
        Logger::Log(Logger::Error, string_format("Texture %s doesn't exist", id.c_str()));
        return false;
    }
    
    ImageTexture texture = GetTexture(id);
    texture.Unload();

    mTextures.erase(id);
    return true;
}