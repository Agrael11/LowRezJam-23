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
#include "TextureManager.h"
#include "SpriteManager.h"
#include "Enemy.h"
#include "LevelData.h"



#include "Levels.h"

#include <fstream>

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
    
    this->mColorBlack = Colorf(0.8f,0.8f,0.8f,1.0f);
    this->mColorDarkGray = Colorf(0.1f,0.1f,0.1f,1.0f);
    this->mColorDarkBlue = Colorf(0.1f,0.1f,1.0f,1.0f);
    this->mColorDarkRed = Colorf(0.3f,0.1f,0.1f,1.0f);
    this->mColorPurple = Colorf(0.8f,0.5f,0.6f,1.0f);
    this->mColorLightGreen = Colorf(0.5f,0.85f,0.5f,1.0f);

    this->scroll = 0.f;
    this->scrollSpeed = 6.f;
    
    //TODO: Prepare reset function for stuff
    this->mCurrentLevel = 0; //TODO: Start with correct level
    this->mTimerStatus = 0;
    this->mLevelStage = 0;
    this->mProgressPerc = 0;
    this->mFirstTime = true;
    Logger::MinimumPrintLevel = Logger::Error;
}

void WriteToFile(std::string path, std::string data)
{
    std::ofstream file(path);
    if (file.is_open())
    {
        file << data;
        file.close();
        Logger::Log(Logger::Info, string_format("Data written to file: %s", path.c_str()));
    }
    else
    {
        Logger::Log(Logger::Error, string_format("Failed to open file for writing: %s", path.c_str()));
    }
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

    TextureManager::LoadTexture("BG_1", "Assets/BG_1.png", this->renderer);
    TextureManager::LoadTexture("Bullets", "Assets/Bullets.png", this->renderer);
    TextureManager::LoadTexture("Ship", "Assets/Ship.png", this->renderer);
    TextureManager::LoadTexture("Enemies", "Assets/Enemies.png", this->renderer);

    LoadLevelFromFile("Assets/Level0.lvl");
    /*std::vector<EnemySpawnInfo> infos;
    infos.push_back(EnemySpawnInfo(Bullet::Type::Static, 200, Vector2f(31, 128), Vector2f(0.f, -1.f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Zigzag, 200, Vector2f(128, 58), Vector2f(-1.f, -0.1f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Zigzag, 20, Vector2f(-63, 58), Vector2f(1.f, -0.1f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Circling, 180, Vector2f(128, 54), Vector2f(-1.f, 0.f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Zigzag, 180, Vector2f(12, 128), Vector2f(0.f, -1.f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Zigzag, 10, Vector2f(45, 128), Vector2f(0.f, -1.f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Zigzag, 180, Vector2f(12, 128), Vector2f(0.f, -1.f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Static, 10, Vector2f(48, 128), Vector2f(0.f, -1.f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Static, 10, Vector2f(31, 128), Vector2f(0.f, -1.f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Circling, 180, Vector2f(-63, 58), Vector2f(1.f, -0.1f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Circling, 10, Vector2f(128, 58), Vector2f(-1.f, -0.1f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Zigzag, 180, Vector2f(-63, 54), Vector2f(1.f, 0.f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Static, 10, Vector2f(128, 58), Vector2f(-1.f, 0.f)));
    infos.push_back(EnemySpawnInfo(Bullet::Type::Zigzag, 20, Vector2f(31, 128), Vector2f(0.f, -1.f)));
    LevelData level(infos,0 ,0);
    WriteToFile("Level_.lvl",level.ToString());*/

    Sprite bgSprite;
    Sprite bulletSprite0;
    Sprite bulletSprite1;
    Sprite bulletSprite2;
    Sprite shipStraight;
    Sprite enemyStraight;
    Sprite enemyZigzag;
    Sprite enemyCircle;
    bgSprite.Load(TextureManager::GetTexture("BG_1"), Rectangle(0,0,8,8), this->renderer);
    bulletSprite0.Load(TextureManager::GetTexture("Bullets"), Rectangle(0, 0, BULLET_WIDTH, BULLET_HEIGHT), this->renderer);
    bulletSprite1.Load(TextureManager::GetTexture("Bullets"), Rectangle(BULLET_WIDTH, 0, BULLET_WIDTH, BULLET_HEIGHT), this->renderer);
    bulletSprite2.Load(TextureManager::GetTexture("Bullets"), Rectangle(BULLET_WIDTH*2, 0, BULLET_WIDTH, BULLET_HEIGHT), this->renderer);
    shipStraight.Load(TextureManager::GetTexture("Ship"), Rectangle(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT), this->renderer);
    enemyStraight.Load(TextureManager::GetTexture("Enemies"), Rectangle(0, 0, ENEMY_WIDTH, ENEMY_HEIGHT), this->renderer);
    enemyZigzag.Load(TextureManager::GetTexture("Enemies"), Rectangle(ENEMY_WIDTH, 0, ENEMY_WIDTH, ENEMY_HEIGHT), this->renderer);
    enemyCircle.Load(TextureManager::GetTexture("Enemies"), Rectangle(2*ENEMY_WIDTH, 0, ENEMY_WIDTH, ENEMY_HEIGHT), this->renderer);

    SpriteManager::AddSprite("BG_1", bgSprite);
    SpriteManager::AddSprite("Bullets_0", bulletSprite0);
    SpriteManager::AddSprite("Bullets_1", bulletSprite1);
    SpriteManager::AddSprite("Bullets_2", bulletSprite2);
    SpriteManager::AddSprite("Ship_Straight", shipStraight);
    SpriteManager::AddSprite("Enemy_Straight", enemyStraight);
    SpriteManager::AddSprite("Enemy_Zigzag", enemyZigzag);
    SpriteManager::AddSprite("Enemy_Circle", enemyCircle);

    Logger::Log(Logger::Fatal, "Beta version, not complete yet!");
    Logger::Log(Logger::Fatal, "Starting at LEVEL 0 instead of LEVEL 5!");
    Logger::Log(Logger::Fatal, "MEOW!");
    Logger::Log(Logger::Fatal, "All ingame informations are shown as FATAL ERRORS currently, not on screen!");
}

bool Game::Update(double delta)
{
    if (!this->mRunning)
    {
        return false;
    }

    if (this->mLevelStage == 0)
    {
        if (this->mFirstTime)
        {
            Logger::Log(Logger::Fatal, "Skipping stage 0 on start.");
            this->mCurrentLevel = 0;
            this->mCurrentEnemy = 0;
            this->mTargetTimer = levels[this->mCurrentLevel].GetEnemySpawnInfo(this->mCurrentEnemy).GetStartTimer();
            this->mLevelStage++;
            this->mTimerStatus = 0;
            this->mFirstTime = false;
            Logger::Log(Logger::Fatal, "Moving to Stage 1 - Level.");
        }
        else
        {
            //TODO: BOSS FIGHT!!;
            Logger::Log(Logger::Fatal, "Skipping as no bossfights are implemented.");
            this->mCurrentLevel = 0;
            this->mCurrentEnemy = 0;
            this->mTargetTimer = levels[this->mCurrentLevel].GetEnemySpawnInfo(this->mCurrentEnemy).GetStartTimer();
            this->mLevelStage++;
            this->mTimerStatus = 0;
            Logger::Log(Logger::Fatal, "Moving to Stage 1 - Level.");
        }
    }
    else if (this->mLevelStage == 1)
    {
        if (this->mTimerStatus >= this->mTargetTimer)
        {
            if (this->mCurrentEnemy != -1)
            { 
                EnemySpawnInfo info = levels[this->mCurrentLevel].GetEnemySpawnInfo(this->mCurrentEnemy);
                enemies.push_back(Enemy(info));
                Logger::Log(Logger::Info, string_format("Spawning enemy: %s", info.ToString(false).c_str()));
                this->mCurrentEnemy++;
                if (levels[this->mCurrentLevel].GetEnemySpawnInfos().size() <= this->mCurrentEnemy)
                {
                    this->mTargetTimer += 250;
                    this->mCurrentEnemy = -1;
                }
                else
                {
                    EnemySpawnInfo info = levels[this->mCurrentLevel].GetEnemySpawnInfo(this->mCurrentEnemy);
                    this->mTargetTimer += info.GetStartTimer();
                }
            }
            else
            {
                this->mLevelStage++;
                Logger::Log(Logger::Fatal, "Moving to Stage 2 - Check.");
            }
        }
        this->mTimerStatus += delta/16.;
    }
    else if (this->mLevelStage == 2)
    {
        bool clear = true;
        for (auto& enemy : enemies)
        {
            clear &= (enemy.IsDestroyed() != 2);
        }
        if (clear) 
        {
            Logger::Log(Logger::Fatal, "Moving to Stage 0 - Level Cleared.");
            Logger::Log(Logger::Fatal, "In Final Version moves to next level.");
        }
        else
        {
            Logger::Log(Logger::Fatal, "Moving to Stage 0 - Level Failed.");
        }
        enemies.clear();
        //DO THE CHECK YAY;
        this->mLevelStage = 0;
    }
    
    this->scroll += this->scrollSpeed * (float)(delta / 1000.f);
    if (this->scroll < 0) this->scroll = 0;
    while (this->scroll > 8) this->scroll -= 8;
    if (this->scroll < 0) this->scroll = 0;

    this->mPlayer.Update(delta);
    for (int i = (int)this->bullets.size()-1; i >= 0; i--)
    {
        this->bullets[i].Update(delta);
        Rectangle bulletRectnagle = this->bullets[i].GetRectangle();
        Rectangle collisionRectnagle;
        if (this->bullets[i].IsOwnedByPlayer())
        {
            for (int j = (int)this->enemies.size()-1; j >= 0; j--)
            {
                if (this->enemies[j].IsDestroyed() == 0)
                {
                    collisionRectnagle = this->enemies[j].GetRectangle();
                    if (collisionRectnagle.Intersects(bulletRectnagle))
                    {
                        //TODO: Do stuff;
                        this->enemies[j].Destroy(1);
                        this->bullets[i].Destroy();
                        break;
                    }
                }
            }
        }
        else
        {
            collisionRectnagle = mPlayer.GetRectangle();
            if (collisionRectnagle.Intersects(bulletRectnagle))
            {
                int hp = this->mPlayer.Hurt(10);
                if (hp <= 0)
                {
                    this->mPlayer.SetHealth(100);
                    Logger::Log(Logger::Fatal, "YOU DIED");
                    Logger::Log(Logger::Fatal, "YOU MAY COLLECT YOUR SOULS ON PLACE YOU DIED AFTER YOU RESPAWN ON BONFIRE");
                    Logger::Log(Logger::Fatal, "wait... that is darksouls");
                    Logger::Log(Logger::Fatal, "Infinite lifes! Dying not working yet!");
                    //TODO: remove life
                }
                this->bullets[i].Destroy();
            }
        }

        if (this->bullets[i].IsDestroyed())
        {
            this->bullets.erase(this->bullets.begin() + i);
            continue;
        }
    }

    for (int i = (int)this->enemies.size()-1; i >= 0; i--)
    {
        if (this->enemies[i].IsDestroyed() == 0)
        {
            this->enemies[i].Update(delta);
            
            if (this->enemies[i].TimerCheck()) 
            {
                this->bullets.push_back(this->enemies[i].SpawnBullet());
            }
        }
    }

    return true;
}

void Game::Draw(double delta)
{
    this->renderer.SetRenderTarget(this->mainRenderTexture);
    this->renderer.Begin();
    this->renderer.SetActiveShader(&this->mShader);
    this->renderer.Clean(this->mColorPurple);
    Rectangle destination = Rectangle(0,64-8,8,8);
    Sprite bgSprite = SpriteManager::GetSprite("BG_1");
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            destination.X = x*8;
            destination.Y = 64-y*8-(int)this->scroll;
            this->renderer.DrawSprite(bgSprite, destination);
        }
    }

    for (int i = 0; i < (int)this->enemies.size(); i++)
    {
        this->enemies[i].Draw(this->renderer);
    }

    for (int i = 0; i < (int)this->bullets.size(); i++)
    {
        this->bullets[i].Draw(this->renderer);
    }

    this->mPlayer.Draw(this->renderer);

    if (this->mLevelStage == 1)
    {
        double perc = this->mTimerStatus / (levels[this->mCurrentLevel].GetLevelLength()+250);
        if (this->mProgressPerc > perc)
        {
            this->mProgressPerc -= delta/16./50.;
        }
        else
        {
            this->mProgressPerc = perc;
        }
        destination.X = 0;
        destination.Y = 62;
        destination.Width = (int)(this->mProgressPerc*64);
        destination.Height = 2;
        this->renderer.DrawSprite(SpriteManager::GetSprite("Bullets_2"), destination);
        destination.X = destination.Width;
        destination.Width = 64 - destination.Width;
        this->renderer.DrawSprite(SpriteManager::GetSprite("Bullets_0"), destination);
    }

    this->renderer.End();

    
    //Draw Main Render Texture
    this->renderer.CleanRenderTarget();
    this->renderer.Begin();
    this->renderer.SetActiveShader(&this->mShader);
    this->renderer.Clean(this->mColorDarkGray);
    destination.X = 0;
    destination.Y = 0;
    destination.Height = 512;
    destination.Width = 512;
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
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        this->MouseButtonDown(e.button);
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
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
            break;  
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
            break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
            break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
            break;
        default:
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
}

void Game::MouseMove(SDL_MouseMotionEvent e)
{
    this->mPlayer.Move(Vector2f(e.xrel*0.1f, -e.yrel*0.1f));
}

void Game::MouseButtonDown(SDL_MouseButtonEvent e)
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        this->bullets.push_back(this->mPlayer.SpawnBullet());
    }
}

void Game::MouseButtonUp(SDL_MouseButtonEvent e)
{
}

void Game::MouseWheelMove(SDL_MouseWheelEvent e)
{
}

void Game::ControllerAxisMove(SDL_ControllerAxisEvent e)
{   
    if (e.axis == SDL_CONTROLLER_AXIS_LEFTX) 
    {
        if (e.value < -DEATH_ZONE || e.value > DEATH_ZONE)
        {
            printf("%d, %d\n", e.axis, e.value);
        }
        if (e.value < -DEATH_ZONE) ;
        else if (e.value > DEATH_ZONE) ;
    }
    else if (e.axis == SDL_CONTROLLER_AXIS_LEFTY)
    {
        if (e.value < -DEATH_ZONE || e.value > DEATH_ZONE)
        {
            printf("%d, %d\n", e.axis, e.value);
        }
        if (e.value < -DEATH_ZONE) ;
        else if (e.value > DEATH_ZONE) ;
    }
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
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            break;  
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            break;
        default:
            break;
    }
}

void Game::ControllerButtonUp(SDL_ControllerButtonEvent e)
{
}