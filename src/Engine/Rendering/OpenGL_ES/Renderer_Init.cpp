#include "../OpenGL/Renderer.h"
#include "../../Helper/Format.h"
#include "../../Helper/Logger.h"

using namespace Engine::Helper;
using namespace Engine::Rendering;

#include "glad/glad.h"

bool Renderer::Init(SDL_Window &window, Uint32 flags    )
{
    this->mWindowReference = &window;

    Logger::Log(Logger::Info, "Creating context."); 
    this->mContext = SDL_GL_CreateContext(&window);
    if (this->mContext == NULL)
    {
        Logger::Log(Logger::Error, string_format("Failed to create context. SDL_Error: %s", SDL_GetError()));
        return false;
    }

    gladLoadGLES2Loader(SDL_GL_GetProcAddress);
    Logger::Log(Logger::Info, "OpenGL Loaded.");
    Logger::Log(Logger::Info, string_format("Vendor:     %s", glGetString(GL_VENDOR)));
    Logger::Log(Logger::Info, string_format("Renderer:   %s", glGetString(GL_RENDERER)));
    Logger::Log(Logger::Info, string_format("Version:    %s", glGetString(GL_VERSION)));

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    int w,h;
    SDL_GetWindowSize(&window, &w, &h);
    SetViewport(0, 0, w, h);
    glClearColor(0.5, 0.5, 0.5, 0);

    return true;
}