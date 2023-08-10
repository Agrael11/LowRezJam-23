#pragma once

#include <vector>

#include "Engine/Rendering/Renderer.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Boss.h"

#define DEATH_ZONE 10000

class GameScene
{
    private:
        enum class LevelStage {Bossfight, Level, Transition};
    
        int mCurrentLevel;
        int mNextLevel;
        LevelStage mLevelStage;
        int mCurrentEnemy;
        int mLives;

        double mTimerStatus;
        double mTargetTimer;
        double mVisualProgress;
        double mVisualPlayerHealth;
        double mVisualPlayerHealthSpeed;
        double mScreenTimer;
        int mScreenToShot;
        bool mStarted;
        bool mShooting;
        float mShootingTimer;
        float mControlX;
        bool mControllerX;
        float mControlY;
        bool mControllerY;
        bool failedShown;

        bool mFirstPlay;

        Player mPlayer;

        float mScroll;
        float mScrollSpeed;

        std::vector<Bullet> mBullets;
        std::vector<Enemy> mEnemies;
        std::vector<int> mBgTiles;
        Boss CurrentBoss;
        
        void WriteToFile(std::string path, std::string data);

    public:
        GameScene();

        void Init();
        void LoadContent(Engine::Rendering::Renderer& renderer);
        int GenerateBgPart();
        void Reset();
        bool Update(double delta);
        bool UpdateBossfight(double delta);
        bool UpdateLevel(double delta);
        bool UpdateTransition(double delta);
        void Draw(double delta, Engine::Rendering::Renderer& renderer);
        void DrawGame(double delta, Engine::Rendering::Renderer& renderer);
        void DrawUI(double delta, Engine::Rendering::Renderer& renderer);
        void Shoot();
        bool HealPlayer(float amount);
        bool HurtPlayer(float amount);

        void KeyUp(SDL_KeyboardEvent e);
        void KeyDown(SDL_KeyboardEvent e);
        void MouseMove(SDL_MouseMotionEvent e);
        void MouseButtonDown(SDL_MouseButtonEvent e);
        void MouseButtonUp(SDL_MouseButtonEvent e);
        void ControllerAxisMove(SDL_ControllerAxisEvent e);
        void ControllerButtonDown(SDL_ControllerButtonEvent e);
        void ControllerButtonUp(SDL_ControllerButtonEvent e);
};