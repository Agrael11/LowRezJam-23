#include "ImageTexture.h"

#include <iostream>

#include "Renderer.h"
#include <SDL.h>
#include <SDL_image.h>

#include "../../Math/Vector2.h"
#include "../../Helper/Logger.h"
#include "../../Helper/Format.h"

using namespace Engine::Math;
using namespace Engine::Helper;
using namespace Engine::Rendering;

bool ImageTexture::Load(std::string filePath, Renderer &renderer)
{
    Logger::Log(Logger::Info, string_format("Loading texture %s...", filePath.c_str()));
    this->mTexture = IMG_LoadTexture(renderer.GetSDL_Renderer(), filePath.c_str());

    if (this->mTexture == NULL)
    {
        Logger::Log(Logger::Error, string_format("Unable to load texture from file %s! SDL Error: %s", filePath.c_str(), SDL_GetError()));
        return false;
    }

    int sizeX = 0;
    int sizeY = 0;
    
    SDL_QueryTexture(this->mTexture, NULL, NULL, &sizeX, &sizeY);

    this->mSize = Vector2(sizeX, sizeY);

    return true;
}

void ImageTexture::Unload()
{
    SDL_DestroyTexture(this->mTexture);
    this->mTexture = NULL;
}