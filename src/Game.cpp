#include <random>
#include <time.h>

#include "Game.h"

#include "Engine/BaseGame.h"
#include "Engine/Helper/Logger.h"
#include "Engine/Helper/format.h"
#include "Engine/Audio/Audio.h"
#include "Engine/Support.h"
#include "Engine/Rendering/ImageTexture.h"

#include "Engine/Math/Color.h"
#include "Engine/Math/Colorf.h"

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
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    //Logger::MinimumPrintLevel = Logger::Error;
    
    this->mGameScene.Init();
}

void Game::LoadContent()
{
    #if USE_OPENGL_ES
    this->mShader.LoadFromFile("Assets/mainShader_es.vert", "Assets/mainShader_es.frag");
    #else
    this->mShader.LoadFromFile("Assets/mainShader.vert", "Assets/mainShader.frag");
    #endif

    this->mainRenderTexture.Create(64, 64, this->renderer);
    this->mainRenderTargetSprite.Load(this->mainRenderTexture, this->renderer);

    this->mGameScene.LoadContent(this->renderer);
}

bool Game::Update(double delta)
{
    if (!this->mRunning)
    {
        return false;
    }

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
    Rectangle destination(0,0,512,512);
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
}


void Game::KeyDown(SDL_KeyboardEvent e)
{
    switch (e.keysym.scancode)
    {
        case SDL_SCANCODE_ESCAPE:
            this->mRunning = false;
            break;
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