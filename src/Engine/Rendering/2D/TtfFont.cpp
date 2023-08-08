#include "TtfFont.h"

#include <SDL_ttf.h>
#include "../../Helper/Format.h"
#include "../../Helper/Logger.h"

using namespace Engine::Rendering;
using namespace Engine::Helper;

bool TtfFont::Load(std::string filePath, int size, Renderer &renderer)
{
    Logger::Log(Logger::Info, string_format("Loading font %s...", filePath.c_str()));
    this->mFont = TTF_OpenFont(filePath.c_str(), size);
    if (this->mFont == NULL)
    {
        Logger::Log(Logger::Error, string_format("Could not load font %s. SDL_ttf Error: %s!", filePath.c_str(), TTF_GetError()));
        return false;
    }

    return true;
}

TTF_Font* TtfFont::GetFont()
{
    return this->mFont;
}

void TtfFont::Unload()
{
    TTF_CloseFont(this->mFont);
    this->mFont = NULL;
}