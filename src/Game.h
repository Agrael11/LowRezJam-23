#pragma once

#include <vector>

#include "Engine/BaseGame.h"
#include "Engine/Rendering/Sprite.h"
#include "Engine/Rendering/RenderTexture.h"
#include "Engine/Rendering/ImageTexture.h"
#include "Engine/Rendering/Shader.h"
#include "Engine/Audio/Music.h"
#include "Engine/Audio/Sound.h"
#include "Engine/Math/Color.h"
#include "Engine/Math/Colorf.h"
#include "Engine/Rendering/TtfFont.h"
#include "Engine/Rendering/FontTexture.h"


#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

#define DEATH_ZONE 10000

class Game: public Engine::BaseGame
{
private:
    bool mRunning;
    bool mPressedF4;
    Engine::Math::Colorf mColorBlack;
    Engine::Math::Colorf mColorDarkGray;
    Engine::Math::Colorf mColorDarkBlue;
    Engine::Math::Colorf mColorDarkRed;
    Engine::Math::Colorf mColorPurple;
    Engine::Math::Colorf mColorLightGreen;
    Engine::Rendering::Shader mShader;
    int mCurrentLevel;
    int mLevelStage;
    int mCurrentEnemy;
    double mTimerStatus;
    double mTargetTimer;
    double mProgressPerc;
    bool mFirstTime;

    Player mPlayer;
    
public:
    Engine::Rendering::RenderTexture mainRenderTexture;
    Engine::Rendering::Sprite mainRenderTargetSprite;
    float scroll;
    float scrollSpeed;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

private:
    void KeyDown(SDL_KeyboardEvent e);
    void KeyUp(SDL_KeyboardEvent e);
    void MouseMove(SDL_MouseMotionEvent e);
    void MouseButtonDown(SDL_MouseButtonEvent e);
    void MouseButtonUp(SDL_MouseButtonEvent e);
    void MouseWheelMove(SDL_MouseWheelEvent e);
    void ControllerAxisMove(SDL_ControllerAxisEvent e);
    void ControllerButtonDown(SDL_ControllerButtonEvent e);
    void ControllerButtonUp(SDL_ControllerButtonEvent e);
public:

    Game(int width, int height, std::string windowTitle);
    void Init();
    void LoadContent();
    void HandleEvent(SDL_Event e);
    bool Update(double deltaTime);
    void Draw(double deltaTime);
    void Exit();
};