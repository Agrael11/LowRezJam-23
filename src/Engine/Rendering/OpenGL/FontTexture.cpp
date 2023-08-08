#include "FontTexture.h"

#include <iostream>

#include "Renderer.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "../../Math/Vector2.h"
#include "../../Helper/Logger.h"
#include "../../Helper/Format.h"

#include "glad/glad.h"

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

    int size = textSurface->w * textSurface->h*4;
    Uint8* data = new Uint8[size];
    for (int x = 0; x < textSurface->w; x++)
    {
        for (int y = 0; y < textSurface->h; y++)
        {
            int i = (textSurface->h-y-1) * textSurface->pitch + x * 4;
            int i2 = (y * textSurface->w + x) * 4;
            Uint8* pix = (Uint8*)textSurface->pixels + i;
            data[i2+0] = *(pix+1);
            data[i2+1] = *(pix+0);
            data[i2+2] = *(pix+2);
            data[i2+3] = *(pix+3);
        }
    }

    glGenTextures(1, &this->mTexture);
    glBindTexture(GL_TEXTURE_2D, this->mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textSurface->w, textSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(textSurface);

    this->mSize = Vector2(textSurface->w, textSurface->h);
    this->mCreated = true;

    delete[] data;

    return true;
}

void FontTexture::Unload()
{
    glDeleteTextures(1, &this->mTexture);
    this->mCreated = false;
}