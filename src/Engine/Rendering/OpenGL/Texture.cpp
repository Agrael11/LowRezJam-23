#include "Texture.h"

#include <iostream>
#include "glad/glad.h"

#include "../../Math/Vector2.h"
#include "../../Helper/Logger.h"
#include "../../Helper/Format.h"

using namespace Engine::Math;
using namespace Engine::Helper;
using namespace Engine::Rendering;

unsigned int Texture::GetHandle()
{
    return this->mTexture;
}

int Texture::GetWidth()
{
    return this->mSize.X;
}

int Texture::GetHeight()
{
    return this->mSize.Y;
}

Vector2 Texture::GetSize()
{
    return this->mSize;
}

void Texture::Unload()
{
    glDeleteTextures(1, &this->mTexture);
}