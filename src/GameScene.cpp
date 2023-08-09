/*
    private:
        int mCurrentLevel;
        int mNextLevel;
        int mLevelStage;
        int mCurrentEnemy;
        int mLives;

        double mTimerStatus;
        double mTargetTimer;
        double mVisualProgress;
        
        bool mFirstPlay;

        Player mPlayer;

        float mScroll;
        float mScrollSpeed;

        std::vector<Bullet> mBullets;
        std::vector<Enemy> mEnemies;
    public:
        void Init();
        void LoadContent();
        void Reset();
        void Update(double delta);
        void Draw(double delta);
        void DrawGame(double delta);
        void DrawUI(double delta);
*/

#include "GameScene.h"

#include "TextureManager.h"
#include "SpriteManager.h"
#include "Engine/Helper/Format.h"
#include "Engine/Helper/Logger.h"
#include "Engine/Math/Rectangle.h"
#include "Engine/Math/Vector2f.h"
#include "Levels.h"
#include "BossAttackData.h"

using namespace Engine::Rendering;
using namespace Engine::Helper;
using namespace Engine::Math;

GameScene::GameScene()
{
}

void GameScene::WriteToFile(std::string path, std::string data)
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


void GameScene::Init()
{    
    this->Reset();
}

void GameScene::LoadContent(Engine::Rendering::Renderer& renderer)
{
    //LOAD TEXTURES
    TextureManager::LoadTexture("BG_1", "Assets/BG_1.png", renderer);
    TextureManager::LoadTexture("Bullets", "Assets/Bullets.png", renderer);
    TextureManager::LoadTexture("Ship", "Assets/Ship.png", renderer);
    TextureManager::LoadTexture("Enemies", "Assets/Enemies.png", renderer);
    TextureManager::LoadTexture("Life", "Assets/Life.png", renderer);
    TextureManager::LoadTexture("Bosses", "Assets/Bosses.png", renderer);

    //LOAD SPRITES
    Sprite bg0Sprite;
    Sprite bg1Sprite;
    Sprite bg2Sprite;
    Sprite bulletSprite0;
    Sprite bulletSprite1;
    Sprite bulletSprite2;
    Sprite shipStraight;
    Sprite enemyStraight;
    Sprite enemyZigzag;
    Sprite enemyCircle;
    Sprite life;
    Sprite boss0;

    bg0Sprite.Load(TextureManager::GetTexture("BG_1"), Rectangle(0,0,8,8), renderer);
    bg1Sprite.Load(TextureManager::GetTexture("BG_1"), Rectangle(8,0,8,8), renderer);
    bg2Sprite.Load(TextureManager::GetTexture("BG_1"), Rectangle(16,0,8,8), renderer);
    bulletSprite0.Load(TextureManager::GetTexture("Bullets"), Rectangle(0, 0, BULLET_WIDTH, BULLET_HEIGHT), renderer);
    bulletSprite1.Load(TextureManager::GetTexture("Bullets"), Rectangle(BULLET_WIDTH, 0, BULLET_WIDTH, BULLET_HEIGHT), renderer);
    bulletSprite2.Load(TextureManager::GetTexture("Bullets"), Rectangle(BULLET_WIDTH*2, 0, BULLET_WIDTH, BULLET_HEIGHT), renderer);
    shipStraight.Load(TextureManager::GetTexture("Ship"), Rectangle(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT), renderer);
    enemyStraight.Load(TextureManager::GetTexture("Enemies"), Rectangle(0, 0, ENEMY_WIDTH, ENEMY_HEIGHT), renderer);
    enemyZigzag.Load(TextureManager::GetTexture("Enemies"), Rectangle(ENEMY_WIDTH, 0, ENEMY_WIDTH, ENEMY_HEIGHT), renderer);
    enemyCircle.Load(TextureManager::GetTexture("Enemies"), Rectangle(2*ENEMY_WIDTH, 0, ENEMY_WIDTH, ENEMY_HEIGHT), renderer);
    life.Load(TextureManager::GetTexture("Life"), Rectangle(0, 0, 4, 4), renderer);
    boss0.Load(TextureManager::GetTexture("Bosses"), Rectangle(0, 0, BOSS_WEIGHT, BOSS_HEIGHT), renderer);

    SpriteManager::AddSprite("BG_0", bg0Sprite);
    SpriteManager::AddSprite("BG_1", bg1Sprite);
    SpriteManager::AddSprite("BG_2", bg2Sprite);
    SpriteManager::AddSprite("Bullets_0", bulletSprite0);
    SpriteManager::AddSprite("Bullets_1", bulletSprite1);
    SpriteManager::AddSprite("Bullets_2", bulletSprite2);
    SpriteManager::AddSprite("Ship_Straight", shipStraight);
    SpriteManager::AddSprite("Enemy_Straight", enemyStraight);
    SpriteManager::AddSprite("Enemy_Zigzag", enemyZigzag);
    SpriteManager::AddSprite("Enemy_Circle", enemyCircle);
    SpriteManager::AddSprite("Life", life);
    SpriteManager::AddSprite("Boss_0", boss0);

    //LOAD LEVELS
    LoadLevelFromFile("Assets/Level0.lvl");

    //TODO: DEBUG DATA
    Logger::Log(Logger::Fatal, "Beta version, not complete yet!");
    Logger::Log(Logger::Fatal, "Starting at LEVEL 0 instead of LEVEL 5!");
    Logger::Log(Logger::Fatal, "MEOW!");
    Logger::Log(Logger::Fatal, "All ingame informations are shown as FATAL ERRORS currently, not on screen!");
}

int GameScene::GenerateBgPart()
{
    int bgbase = rand()%100;
    if (bgbase < 50) return 0;
    if (bgbase < 85) return 1;
    return 2;
}

void GameScene::Reset()
{
    this->mScroll = 0.f;
    this->mScrollSpeed = 6.f;
    
    this->mCurrentLevel = 0; //TODO: Start with correct level
    this->mNextLevel = 0;
    this->mTimerStatus = 0;
    this->mLevelStage = LevelStage::Bossfight;
    this->mVisualProgress = 0;
    this->mLives = 3;
    this->mFirstPlay = true;
    this->mVisualPlayerHealth = 100.f;
    this->mVisualPlayerHealthSpeed = 1.f;
    this->mBgTiles.clear();
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            this->mBgTiles.push_back(this->GenerateBgPart());
        }
    }
    this->CurrentBoss.Recycle(0);
}

bool GameScene::Update(double delta)
{
    switch (this->mLevelStage)
    {
        case LevelStage::Bossfight:
            this->UpdateBossfight(delta);
            break;
        case LevelStage::Level:
            this->UpdateLevel(delta);
            break;
        case LevelStage::Transition:
            this->UpdateTransition(delta);
            break;
        default:
            Logger::Log(Logger::Fatal, "Wrong Level State");
            break;
    }
    
    this->mScroll += this->mScrollSpeed * (float)(delta / 1000.f);
    while (this->mScroll > 8)
    {
        this->mScroll -= 8;
        for (int i = 0; i < 8; i++)
        {
            this->mBgTiles.erase(this->mBgTiles.begin());
            this->mBgTiles.push_back(this->GenerateBgPart());
        }
    }
    if (this->mScroll < 0)
    {
        this->mScroll = 0;
    }

    this->mPlayer.Update(delta);


    for (int i = (int)this->mBullets.size()-1; i >= 0; i--)
    {
        this->mBullets[i].Update(delta);
        Rectangle bulletRectnagle = this->mBullets[i].GetRectangle();
        Rectangle collisionRectnagle;
        if (this->mBullets[i].IsOwnedByPlayer())
        {
            if (this->mLevelStage == LevelStage::Level)
            {
                for (int j = (int)this->mEnemies.size()-1; j >= 0; j--)
                {
                    if (this->mEnemies[j].IsDestroyed() == 0)
                    {
                        collisionRectnagle = this->mEnemies[j].GetRectangle();
                        if (collisionRectnagle.Intersects(bulletRectnagle))
                        {
                            //TODO: Do stuff;
                            this->mEnemies[j].Destroy(1);
                            this->mBullets[i].Destroy();
                            break;
                        }
                    }
                }
            }
            else if (this->mLevelStage == LevelStage::Bossfight)
            {
                collisionRectnagle = this->CurrentBoss.GetRectangle();
                if (collisionRectnagle.Intersects(bulletRectnagle))
                {
                    //TODO: Do stuff;
                    this->CurrentBoss.SetHealth(this->CurrentBoss.GetHealth()-2);
                    this->mBullets[i].Destroy();
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
                    this->mLives--;
                    if (this->mLives < 0)
                    {
                        this->mLives=3;
                        Logger::Log(Logger::Fatal, "Lies don't really work now. RESET TO 3");
                    }
                    //TODO: remove life
                }
                this->mBullets[i].Destroy();
            }
        }

        if (this->mBullets[i].IsDestroyed())
        {
            this->mBullets.erase(this->mBullets.begin() + i);
            continue;
        }
    }

    double playerHealthDifference = this->mVisualPlayerHealth - this->mPlayer.GetHealth();
    if (abs(playerHealthDifference) < this->mVisualPlayerHealthSpeed)
    {
        this->mVisualPlayerHealth = this->mPlayer.GetHealth();
    }
    else if (playerHealthDifference < 0)
    {
        this->mVisualPlayerHealth += this->mVisualPlayerHealthSpeed * delta / 16.f * 2;
    }
    else if (playerHealthDifference > 0)
    {
        this->mVisualPlayerHealth -= this->mVisualPlayerHealthSpeed * delta / 16.f * 2;
    }

    return true;
}

bool GameScene::UpdateBossfight(double delta)
{
    if (this->mFirstPlay)
    {
        Logger::Log(Logger::Fatal, "Skipping stage 0 on start.");
        this->mCurrentLevel = 0;
        this->mCurrentEnemy = 0;
        this->mTargetTimer = levels[this->mCurrentLevel].GetEnemySpawnInfo(this->mCurrentEnemy).GetStartTimer();
        this->mLevelStage = LevelStage::Level;
        this->mTimerStatus = 0;
        this->mFirstPlay = false;
        Logger::Log(Logger::Fatal, "Moving to Stage 1 - Level.");
    }
    else
    {
        if (this->CurrentBoss.GetHealth() <= 0)
        {
            this->mCurrentLevel = 0;
            this->mCurrentEnemy = 0;
            this->mTargetTimer = levels[this->mCurrentLevel].GetEnemySpawnInfo(this->mCurrentEnemy).GetStartTimer();
            this->mLevelStage = LevelStage::Level;
            this->mTimerStatus = 0;
            Logger::Log(Logger::Fatal, "Moving to Stage 1 - Level.");
            int hp = this->mPlayer.GetHealth();
            hp += 50;
            if (hp > 100)
            {
                if (this->mLives > 5)
                {
                    hp = 100;
                }
                else
                {
                    hp -= 100;
                    this->mLives += 1;
                }
                this->mPlayer.SetHealth(hp);
            }
        }
        else
        {
            this->CurrentBoss.Update(delta);
            
            double perc = this->CurrentBoss.GetHealth() / 100.;
            if (this->mVisualProgress > perc)
            {
                this->mVisualProgress -= delta/16./50.;
            }
            else
            {
                this->mVisualProgress = perc;
            }
            
            std::vector<Bullet> spawnedBullets = this->CurrentBoss.SpawnBullets();
            this->mBullets.insert(this->mBullets.end(), std::make_move_iterator(spawnedBullets.begin()), std::make_move_iterator(spawnedBullets.end()));
            spawnedBullets.clear();
        }
        //TODO: BOSS FIGHT!!;
    }

    return true;
}

bool GameScene::UpdateLevel(double delta)
{
    if (this->mTimerStatus >= this->mTargetTimer)
    {
        if (this->mCurrentEnemy != -1)
        { 
            EnemySpawnInfo info = levels[this->mCurrentLevel].GetEnemySpawnInfo(this->mCurrentEnemy);
            this->mEnemies.push_back(Enemy(info));
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
            this->mLevelStage = LevelStage::Transition;
            bool clear = true;
            for (auto& enemy : this->mEnemies)
            {
                clear &= (enemy.IsDestroyed() == 1);
            }
            if (clear) 
            {
                Logger::Log(Logger::Fatal, "Moving to Stage 2 - Transition - Level Cleared.");
                Logger::Log(Logger::Fatal, "In Final Version moves to next level.");
            }
            else
            {
                Logger::Log(Logger::Fatal, "Moving to Stage 2 - Transition - Level Failed.");
            }
        }
    }
    this->mTimerStatus += delta/16.;
    
    for (int i = (int)this->mEnemies.size()-1; i >= 0; i--)
    {
        if (this->mEnemies[i].IsDestroyed() == 0)
        {
            this->mEnemies[i].Update(delta);

            Rectangle playerRectangle = this->mPlayer.GetRectangle();
            if (this->mEnemies[i].GetRectangle().Intersects(playerRectangle))
            {
                this->mEnemies[i].Destroy(2);
                this->mEnemies.erase(this->mEnemies.begin()+i);
                this->mPlayer.Hurt(10);
                continue;
            }
            
            if (this->mEnemies[i].TimerCheck()) 
            {
                this->mBullets.push_back(this->mEnemies[i].SpawnBullet());
            }
        }
    }

    double perc = this->mTimerStatus / (levels[this->mCurrentLevel].GetLevelLength()+250);
    if (this->mVisualProgress > perc)
    {
        this->mVisualProgress -= delta/16./50.;
    }
    else
    {
        this->mVisualProgress = perc;
    }

    return true;
}

bool GameScene::UpdateTransition(double delta)
{
    Logger::Log(Logger::Fatal, "Moving to Stage 0 - Boss. Transition not done yet. Normally would move to next level");
    this->mEnemies.clear();
    //TODO: Move to next level
    this->mLevelStage = LevelStage::Bossfight;
    return true;
}

void GameScene::Draw(double delta, Engine::Rendering::Renderer& renderer)
{
    DrawGame(delta, renderer);
    DrawUI(delta, renderer);
}

void GameScene::DrawGame(double delta, Engine::Rendering::Renderer& renderer)
{
    Rectangle destination = Rectangle(0,64-8,8,8);
    
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 9; y++)
        {
            int tile = this->mBgTiles[(8-y)*8+x];
            destination.X = x*8;
            destination.Y = 64-y*8-(int)this->mScroll;
            renderer.DrawSprite(SpriteManager::GetSprite(string_format("BG_%d",tile).c_str()), destination);
        }
    }

    for (int i = 0; i < (int)this->mBullets.size(); i++)
    {
        this->mBullets[i].Draw(renderer);
    }

    if (this->mLevelStage == LevelStage::Bossfight)
    {
        this->CurrentBoss.Draw(renderer);
    }

    else if (this->mLevelStage == LevelStage::Level)
    {
        for (int i = 0; i < (int)this->mEnemies.size(); i++)
        {
            this->mEnemies[i].Draw(renderer);
        }
    }

    this->mPlayer.Draw(renderer);
}

void GameScene::DrawUI(double delta, Engine::Rendering::Renderer& renderer)
{
    Rectangle destination = Rectangle(0,62,(int)(this->mVisualProgress*64),2);
    // Level Progress
    if (this->mLevelStage == LevelStage::Level)
    {
        renderer.DrawSprite(SpriteManager::GetSprite("Bullets_2"), destination);
        destination.X = destination.Width;
        destination.Width = 64 - destination.Width;
        renderer.DrawSprite(SpriteManager::GetSprite("Bullets_0"), destination);
    }
    else if (this->mLevelStage == LevelStage::Bossfight) // Boss Health
    {
        renderer.DrawSprite(SpriteManager::GetSprite("Bullets_1"), destination);
        destination.X = destination.Width;
        destination.Width = 64 - destination.Width;
        renderer.DrawSprite(SpriteManager::GetSprite("Bullets_0"), destination);
    }

    //Player HP bar
    destination = Rectangle(0, 0, (int)(64*this->mVisualPlayerHealth/100.f), 2);
    renderer.DrawSprite(SpriteManager::GetSprite((this->mVisualPlayerHealth > 66)?"Bullets_2":((this->mVisualPlayerHealth > 33)?"Bullets_0":"Bullets_1")) , destination);
    
    //Player lifes
    for (int i = 0; i < this->mLives; i++)
    {
        destination.X = i*5;
        destination.Y = 2;
        destination.Width = 4;
        destination.Height = 4;
        renderer.DrawSprite(SpriteManager::GetSprite("Life"), destination);
    }
}




void GameScene::KeyDown(SDL_KeyboardEvent e)
{
    switch (e.keysym.scancode)
    {
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

void GameScene::KeyUp(SDL_KeyboardEvent e)
{
}

void GameScene::MouseMove(SDL_MouseMotionEvent e)
{
    this->mPlayer.Move(Vector2f(e.xrel*0.1f, -e.yrel*0.1f));
}

void GameScene::MouseButtonDown(SDL_MouseButtonEvent e)
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        this->mBullets.push_back(this->mPlayer.SpawnBullet());
    }
}

void GameScene::MouseButtonUp(SDL_MouseButtonEvent e)
{
}

void GameScene::ControllerAxisMove(SDL_ControllerAxisEvent e)
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

void GameScene::ControllerButtonDown(SDL_ControllerButtonEvent e)
{
    switch (e.button)
    {
        case SDL_CONTROLLER_BUTTON_BACK:
            break;
        case SDL_CONTROLLER_BUTTON_START:
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

void GameScene::ControllerButtonUp(SDL_ControllerButtonEvent e)
{
}