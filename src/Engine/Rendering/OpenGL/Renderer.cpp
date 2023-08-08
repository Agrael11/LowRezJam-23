#include "Renderer.h"
#include "../../Helper/format.h"
#include "../../Helper/Logger.h"

#include "../../Math/Color.h"
#include "../../Math/Colorf.h"

using namespace Engine::Helper;
using namespace Engine::Math;
using namespace Engine::Rendering;

#include "glad/glad.h"

SDL_Renderer* Renderer::GetSDL_Renderer()
{
    return NULL;
}

void Renderer::Begin()
{

}

void Renderer::End()
{
    if (!this->mRenderTexture)
    {
        SDL_GL_SwapWindow(this->mWindowReference);
    }
}

bool Renderer::DrawSprite(Sprite &sprite, Rectangle &destination, Rectangle *source, double rotation, bool flipHorizontal, bool flipVertical)
{
    if (source == NULL)
    {
        return sprite.Draw(destination, *this, rotation, flipHorizontal, flipVertical);
    }
    return sprite.Draw(*source, destination, *this, rotation, flipHorizontal, flipVertical);
}

void Renderer::Clean(Color &color)
{
    Colorf colorf = color.GetColorf();
    glClearColor(colorf.R, colorf.G, colorf.B, colorf.A);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Clean(Colorf &color)
{
    glClearColor(color.R, color.G, color.B, color.A);
    glClear(GL_COLOR_BUFFER_BIT);
}
    
void Renderer::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
    this->mViewport.X = x;
    this->mViewport.Y = y;
    this->mViewport.Width = width;
    this->mViewport.Height = height;
}

    
Rectangle Renderer::GetViewport()
{
    return this->mViewport;
}

bool Renderer::SetRenderTarget(RenderTexture &texture)
{
    this->mRenderTexture = true;
    return texture.SetAsRenderTarget(*this);
}
bool Renderer::CleanRenderTarget()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->mRenderTexture = false;
    return true;
}

void Renderer::Destroy()
{
    SDL_GL_DeleteContext(this->mContext);
    this->mContext = NULL;
}

void Renderer::SetActiveShader(Shader* shader)
{
    this->mActiveShader = shader;
}

Shader* Renderer::GetActiveShader()
{
    return this->mActiveShader;
}