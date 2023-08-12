#include "RenderTexture.h"
#include "Renderer.h"
#include "../../Helper/Logger.h"
#include "../../Helper/Format.h"
#include "../../Math/MathHelper.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace Engine::Helper;
using namespace Engine::Rendering;
using namespace Engine::Math;

bool RenderTexture::Create(int width, int height, Renderer &renderer)
{
    glGenFramebuffers(1, &this->mFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->mFrameBuffer);

    if (!glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        Logger::Log(Logger::Error, "Unable to create render texture!");
        return false;
    }

    glGenTextures(1, &this->mTexture);
    glBindTexture(GL_TEXTURE_2D, this->mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mTexture, 0);

    this->mSize = Vector2(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    return true;
}

bool RenderTexture::SetAsRenderTarget(Renderer &renderer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->mFrameBuffer);
    return true;
}

void RenderTexture::Unload()
{
    glDeleteFramebuffers(1, &this->mFrameBuffer);
    glDeleteTextures(1, &this->mTexture);
}