#include "RenderTexture.h"
#include "../../Helper/Logger.h"
#include "../../Helper/Format.h"
#include "Renderer.h"
#include "../../Math/MathHelper.h"

using namespace Engine::Helper;
using namespace Engine::Rendering;
using namespace Engine::Math;

bool RenderTexture::Create(int width, int height, Renderer &renderer)
{
    this->mAccess = SDL_TEXTUREACCESS_TARGET;
    this->mTexture = SDL_CreateTexture(renderer.GetSDL_Renderer(), SDL_PIXELFORMAT_RGBA8888, this->mAccess, width, height);

    if (this->mTexture == NULL)
    {
        Logger::Log(Logger::Error, string_format("Unable to create render texture! SDL Error: %s", SDL_GetError()));
        return false;
    }
    
    this->mSize = Vector2(width, height);

    return true;
}

bool RenderTexture::SetAsRenderTarget(Renderer &renderer)
{
    if (SDL_SetRenderTarget(renderer.GetSDL_Renderer(), this->mTexture) != 0)
    {
        Logger::Log(Logger::Error, string_format("Unable to set render texture! SDL Error: %s", SDL_GetError()));
        return false;
    }
    return true;
}

void RenderTexture::Unload()
{
    SDL_DestroyTexture(this->mTexture);
    this->mTexture = NULL;
}