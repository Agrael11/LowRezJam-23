#include "FontTexture.h"

#include <iostream>

#include "Renderer.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "../../Math/Vector2.h"
#include "../../Helper/Logger.h"
#include "../../Helper/Format.h"

using namespace Engine::Math;
using namespace Engine::Helper;
using namespace Engine::Rendering;

bool FontTexture::Generate(std::string text, TtfFont font, Engine::Math::Color color, Renderer &renderer, int wrap)
{
    if (this->mCreated)
    {
        this->Unload();
    }

    SDL_Surface* textSurface = NULL;
    if (wrap == -1)
    {
        textSurface = TTF_RenderUTF8_Blended(font.GetFont(), text.c_str(), color.GetSDL_Color());
    }
    else
    {
        textSurface = TTF_RenderUTF8_Blended_Wrapped(font.GetFont(), text.c_str(), color.GetSDL_Color(), wrap);
    }
    

    if (textSurface == NULL)
    {
        Logger::Log(Logger::Error, string_format("Unable to generate texture from font! SDL Error: %s", SDL_GetError()));
        return false;
    }

    this->mTexture = SDL_CreateTextureFromSurface(renderer.GetSDL_Renderer(), textSurface);

    if (this->mTexture == NULL)
    {
        Logger::Log(Logger::Error, string_format("Unable to generate texture from font! SDL Error: %s", SDL_GetError()));
        return false;
    }

    this->mSize = Vector2(textSurface->w, textSurface->h);
    this->mCreated = true;

    return true;
}

void FontTexture::Unload()
{
    SDL_DestroyTexture(this->mTexture);
    this->mTexture = NULL;
    this->mCreated = false;
}