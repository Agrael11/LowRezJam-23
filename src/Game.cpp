#include <random>
#include <time.h>

#include "Game.h"

#include "Engine/BaseGame.h"
#include "Engine/Helper/Logger.h"
#include "Engine/Helper/Format.h"
#include "Engine/Audio/Audio.h"
#include "Engine/Support.h"
#include "Engine/Rendering/ImageTexture.h"

#include "Engine/Math/Color.h"
#include "Engine/Math/Colorf.h"

#if EMSCRIPTEN
#include "emscripten.h"
#endif

using namespace Engine::Helper;
using namespace Engine::Math;
using namespace Engine::Audio;
using namespace Engine::Rendering;

Game::Game(int width, int height, std::string windowTitle)
{
    this->Load(width, height, windowTitle);
    this->mRunning = true;
}

void Game::Init()
{
    if (Engine::Support::audio)
    {
        InitAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    }
    
    Logger::MinimumPrintLevel = Logger::Error;
    
    this->mGameScene.Init();
}

void Game::LoadContent()
{
    #if USE_OPENGL_ES
    this->mShader.LoadFromFile("Assets/Shaders/mainShader_es.vert", "Assets/Shaders/mainShader_es.frag");
    #else
    this->mShader.LoadFromFile("Assets/Shaders/mainShader.vert", "Assets/Shaders/mainShader.frag");
    #endif

    this->mainRenderTexture.Create(64, 64, this->renderer);
    this->mainRenderTargetSprite.Load(this->mainRenderTexture, this->renderer);

    this->mGameScene.LoadContent(this->renderer);
}

#if EMSCRIPTEN
EM_JS(int, canvas_get_width, (), {
    return window.innerWidth;
});

EM_JS(int, canvas_get_height, (), {
    return window.innerHeight;
});

EM_JS(void, focusWindow, (), {
    window.focus();
});
#endif

bool Game::Update(double delta)
{
    if (!this->mRunning)
    {
        return false;
    }

    #if EMSCRIPTEN
        int width = canvas_get_width();
        int height = canvas_get_height();
        if (width != this->windowWidth || height != this->windowHeight)
        {
            this->SetWindowSize(width, height);
            this->renderer.SetViewport(0, 0, width, height);
        }
    #endif

    if (!this->mGameScene.Update(delta))
    {
        this->mRunning = false;
    }

    return true;
}

void Game::Draw(double delta)
{
    this->renderer.SetRenderTarget(this->mainRenderTexture);
    this->renderer.Begin();
    this->renderer.SetActiveShader(&this->mShader);
    this->renderer.Clean(this->mColorPurple);
    this->mGameScene.Draw(delta, this->renderer);
    this->renderer.End();

    this->renderer.CleanRenderTarget();
    this->renderer.Begin();
    this->renderer.SetActiveShader(&this->mShader);
    this->renderer.Clean(this->mColorDarkGray);
    int xOff = 0;
    int yOff = 0;
    int width = this->windowWidth;
    if (width > this->windowHeight)
    {
        width = this->windowHeight;
        xOff = (this->windowWidth - this->windowHeight) / 2;
    }
    int height = this->windowHeight;
    if (height > this->windowWidth)
    {
        height = this->windowWidth;
        yOff = (this->windowHeight - this->windowWidth) / 2;
    }
    Rectangle destination(xOff, yOff, width, height);
    this->renderer.DrawSprite(this->mainRenderTargetSprite, destination);
    this->renderer.End();
}

void Game::Exit()
{
    QuitAudio();
}

void Game::HandleEvent(SDL_Event e)
{
    if (e.type == SDL_QUIT)
    {
        this->mRunning = false;
    }
    else if( e.type == SDL_KEYDOWN )
    {
        this->KeyDown(e.key);
    }
    else if (e.type == SDL_KEYUP)
    {
        this->KeyUp(e.key);
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
        this->MouseMove(e.motion);
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        #if EMSCRIPTEN
        focusWindow();
        #endif
        this->MouseButtonDown(e.button);
    }
    else if (e.type == SDL_MOUSEBUTTONUP)
    {
        this->MouseButtonUp(e.button);
    }
    else if (e.type == SDL_MOUSEWHEEL)
    {
        this->MouseWheelMove(e.wheel);
    }
    else if (e.type == SDL_CONTROLLERAXISMOTION)
    {
        if (Engine::Support::controller)
        {
            this->ControllerAxisMove(e.caxis);
        }
    }
    else if (e.type == SDL_CONTROLLERBUTTONDOWN)
    {
        if (Engine::Support::controller)
        {
            this->ControllerButtonDown(e.cbutton);
        }
    }
    else if (e.type == SDL_CONTROLLERBUTTONUP)
    {
        if (Engine::Support::controller)
        {
            this->ControllerButtonUp(e.cbutton);
        }
    }
    else if (e.type == SDL_WINDOWEVENT)
    {
        if (e.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            int width = e.window.data1;
            int height = e.window.data2;
            if (width < 64) width = 64;
            if (height < 64) height = 64;
            this->windowWidth = width;
            this->windowHeight = height;
            this->renderer.SetViewport(0, 0, this->windowWidth, this->windowHeight);
        }
    }
}


void Game::KeyDown(SDL_KeyboardEvent e)
{
    switch (e.keysym.scancode)
    {
        case SDL_SCANCODE_F4:
            this->mPressedF4 = true;
            break;
        default:
            this->mGameScene.KeyDown(e);
            break;
    }
}

void Game::KeyUp(SDL_KeyboardEvent e)
{
    if (e.keysym.scancode == SDL_SCANCODE_F4)
    {
        if (this->mPressedF4)
        {
            ToggleFullscreen();
            this->mPressedF4 = false;
        }
    }
    this->mGameScene.KeyUp(e);
}

void Game::MouseMove(SDL_MouseMotionEvent e)
{
    this->mGameScene.MouseMove(e);
}

void Game::MouseButtonDown(SDL_MouseButtonEvent e)
{
    this->mGameScene.MouseButtonDown(e);
}

void Game::MouseButtonUp(SDL_MouseButtonEvent e)
{
    this->mGameScene.MouseButtonUp(e);
}

void Game::MouseWheelMove(SDL_MouseWheelEvent e)
{
}

void Game::ControllerAxisMove(SDL_ControllerAxisEvent e)
{   
    this->mGameScene.ControllerAxisMove(e);
}

void Game::ControllerButtonDown(SDL_ControllerButtonEvent e)
{
    switch (e.button)
    {
        case SDL_CONTROLLER_BUTTON_BACK:
            this->mRunning = false;
            break;
        case SDL_CONTROLLER_BUTTON_START:
            this->mPressedF4 = true;
            break;
        default:
            this->mGameScene.ControllerButtonDown(e);
            break;
    }
}

void Game::ControllerButtonUp(SDL_ControllerButtonEvent e)
{
    this->mGameScene.ControllerButtonUp(e);
}