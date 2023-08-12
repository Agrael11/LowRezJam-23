#include "Renderer.h"
#include "../../Helper/Format.h"
#include "../../Helper/Logger.h"

using namespace Engine::Helper;
using namespace Engine::Rendering;

SDL_Renderer* Renderer::GetSDL_Renderer()
{
    return this->mRenderer;
}

void Renderer::Init(SDL_Window &window, Uint32 flags)
{
    this->mRenderer = SDL_CreateRenderer(&window, -1, SDL_RENDERER_SOFTWARE);
}

void Renderer::Begin()
{

}

void Renderer::End()
{
    SDL_RenderPresent(this->mRenderer);
}

bool Renderer::DrawSprite(Sprite &sprite, Rectangle &destination, Rectangle *source, double rotation, bool flipHorizontal, bool flipVertical)
{
    if (source == NULL)
    {
        //sprite.Draw(destination, *this, rotation, flipHorizontal, flipVertical);
    }
    return false;//sprite.Draw(*source, destination, *this, rotation, flipHorizontal, flipVertical);
}

void Renderer::Clean(Color &color)
{
    SDL_SetRenderDrawColor(this->mRenderer, color.R, color.G, color.B, color.A);
    SDL_RenderClear(this->mRenderer);
}

void Renderer::Clean(Colorf &color)
{
    Color tempColor = color.GetColor();
    SDL_SetRenderDrawColor(this->mRenderer, tempColor.R, tempColor.G, tempColor.B, tempColor.A);
    SDL_RenderClear(this->mRenderer);
}
    
void Renderer::SetViewport(int x, int y, int width, int height)
{
    this->mViewport.X = x;
    this->mViewport.Y = y;
    this->mViewport.Width = width;
    this->mViewport.Height = height;
    SDL_Rect viewport;
    viewport.x = x;
    viewport.y = y;
    viewport.w = width;
    viewport.h = height;
    SDL_RenderSetViewport(this->mRenderer, &viewport);
}

Rectangle Renderer::GetViewport()
{
    return this->mViewport;
}

bool Renderer::SetRenderTarget(RenderTexture &texture)
{
    return texture.SetAsRenderTarget(*this);
}
bool Renderer::CleanRenderTarget()
{
    if (SDL_SetRenderTarget(this->mRenderer, NULL) != 0)
    {
        Logger::Log(Logger::Error, string_format("Unable to clean render texture! SDL Error: %s", SDL_GetError()));
        return false;
    }
    return true;
}

void Renderer::Destroy()
{
    SDL_DestroyRenderer(this->mRenderer);
    this->mRenderer = NULL;
}

void Renderer::SetActiveShader(Shader* shader)
{
    this->mActiveShader = shader;
    this->mActiveShader->UseShader();
}

Shader* Renderer::GetActiveShader()
{
    return this->mActiveShader;
}