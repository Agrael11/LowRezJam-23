#include "ImageTexture.h"

#include <iostream>
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../../Math/Vector2.h"
#include "../../Helper/Logger.h"
#include "../../Helper/Format.h"

using namespace Engine::Math;
using namespace Engine::Helper;
using namespace Engine::Rendering;

bool ImageTexture::Load(std::string filePath, Renderer &renderer)
{
    Logger::Log(Logger::Info, string_format("Loading texture %s...", filePath.c_str()));
    this->name = filePath;

    int sizeX = 0;
    int sizeY = 0;
    int nrChannels;

    glGenTextures(1, &this->mTexture);
    glBindTexture(GL_TEXTURE_2D, this->mTexture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &sizeX, &sizeY, &nrChannels, 0);
    if (data)
    {
        if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);   
        }
    }
    else
    {
        Logger::Log(Logger::Error, "Failed to load texture.");
    }

    this->mSize = Vector2(sizeX, sizeY);

    stbi_image_free(data);

    return true;
}

void ImageTexture::Unload()
{
    glDeleteTextures(1, &this->mTexture);
}