#include "Sprite.h"
#include "../../Helper/Logger.h"
#include "../../Helper/format.h"
#include "Renderer.h"
#include "../../Math/MathHelper.h"
#include "../../Math/Color.h"

using namespace Engine::Helper;
using namespace Engine::Rendering;
using namespace Engine::Math;

bool Sprite::Load(Texture texture, Renderer &renderer)
{
    this->mTexture = texture;

    this->mSize = this->mTexture.GetSize();
    this->sourceRectangle = Rectangle(0, 0, this->mSize.X, this->mSize.Y);
    this->origin = Vector2f(0,0);

    this->mColor.R = 1;
    this->mColor.G = 1;
    this->mColor.B = 1;
    this->mColor.A = 1;

    return true;
}

bool Sprite::Load(Texture texture, Rectangle sourceRectangle, Renderer &renderer)
{
    this->mTexture = texture;

    this->mSize = Vector2(0, 0);
    this->sourceRectangle = sourceRectangle;
    this->origin = Vector2f(0,0);

    this->mColor.R = 1;
    this->mColor.G = 1;
    this->mColor.B = 1;
    this->mColor.A = 1;

    return true;
}

Texture* Sprite::GetTexture()
{
    return &this->mTexture;
}

void Sprite::SetTexture(Texture texture)
{
    this->mTexture = texture;

    this->mSize = this->mTexture.GetSize();
    this->sourceRectangle = Rectangle(0, 0, this->mSize.X, this->mSize.Y);
}

void Sprite::SetTexture(Texture texture, Rectangle sourceRectangle)
{
    this->mTexture = texture;

    this->mSize = this->mTexture.GetSize();
    this->sourceRectangle = sourceRectangle;
}

void Sprite::SetOrigin(float x, float y)
{
    this->origin.X = x;
    this->origin.Y = y;
}

int Sprite::GetWidth()
{
    return this->mSize.X;
}

int Sprite::GetHeight()
{
    return this->mSize.Y;
}

float Sprite::GetRatio()
{
    return (float)this->mSize.X / (float)this->mSize.Y;
}

Vector2* Sprite::GetSize()
{
    return &this->mSize;
}

bool Sprite::Draw(Rectangle &destinationRectangle, Renderer &renderer, double rotationRad, bool flipHorizontal, bool flipVertical)
{
    SDL_Point actualOrigin;
    actualOrigin.x = (int)(destinationRectangle.Width * this->origin.X);
    actualOrigin.y = (int)(destinationRectangle.Height * this->origin.Y);

    rotationRad = MathHelper::RadToDeg(rotationRad);

    int flip = ((flipHorizontal) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | ((flipVertical) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);

    SDL_Rect srcRect = this->sourceRectangle.GetSDL_Rect();
    SDL_Rect dstRect = destinationRectangle.GetSDL_Rect();

    if (SDL_RenderCopyEx(renderer.GetSDL_Renderer(), this->mTexture.GetHandle(), &srcRect, &dstRect, rotationRad, &actualOrigin, (SDL_RendererFlip)flip) != 0)
    {
        Logger::Log(Logger::Error, string_format("Unable to draw texture! SDL Error: %s", SDL_GetError()));
        return false;
    }
    return true;
}

bool Sprite::Draw(Rectangle &sourceRectangle, Rectangle &destinationRectangle, Renderer &renderer, double rotationRad, bool flipHorizontal, bool flipVertical)
{
    SDL_Point actualOrigin;
    actualOrigin.x = (int)(destinationRectangle.Width * this->origin.X);
    actualOrigin.y = (int)(destinationRectangle.Height * this->origin.Y);

    rotationRad = MathHelper::RadToDeg(rotationRad);

    int flip = ((flipHorizontal) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | ((flipVertical) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);

    SDL_Rect srcRect = sourceRectangle.GetSDL_Rect();
    SDL_Rect dstRect = destinationRectangle.GetSDL_Rect();

    if (SDL_RenderCopyEx(renderer.GetSDL_Renderer(), this->mTexture.GetHandle(), &srcRect, &dstRect, rotationRad, &actualOrigin, (SDL_RendererFlip)flip) != 0)
    {
        Logger::Log(Logger::Error, string_format("Unable to draw texture! SDL Error: %s", SDL_GetError()));
        return false;
    }
    return true;
}

bool Sprite::SetBlendMode(BlendMode::BlendMode mode)
{
    SDL_BlendMode bMode;
    switch (mode)
    {
        case BlendMode::Add:
        bMode = SDL_BLENDMODE_ADD;
        break;
        case BlendMode::Mod:
        bMode = SDL_BLENDMODE_MOD;
        break;
        case BlendMode::Blend:
        bMode = SDL_BLENDMODE_BLEND;
        break;
        default:
        case BlendMode::None:
        bMode = SDL_BLENDMODE_NONE;
        break;
    }
    if (SDL_SetTextureBlendMode(this->mTexture.GetHandle(), bMode) != 0)
    {
        Logger::Log(Logger::Error, string_format("Unable to set texture blend mode! SDL Error: %s", SDL_GetError()));
        return false;
    }
    return true;
}

bool Sprite::SetColorMod(Colorf &color)
{
    Color tempC = color.GetColor();
    if (SDL_SetTextureColorMod(this->mTexture.GetHandle(), tempC.R, tempC.G, tempC.B) != 0)
    {
        Logger::Log(Logger::Error, string_format("Unable to set texture color mod! SDL Error: %s", SDL_GetError()));
        return false;
    }
        if (SDL_SetTextureAlphaMod(this->mTexture.GetHandle(), tempC.A) != 0)
    {
        Logger::Log(Logger::Error, string_format("Unable to set texture alpha mod! SDL Error: %s", SDL_GetError()));
        return false;
    }

    this->mColor = color;

    return true;
}

void Sprite::Unload()
{
    this->mTexture.Unload();
}