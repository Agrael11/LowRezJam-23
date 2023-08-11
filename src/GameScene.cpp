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
#include "Engine/Math/MathHelper.h"

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
    TextureManager::LoadTexture("LevelUp", "Assets/LevelUp.png", renderer);
    TextureManager::LoadTexture("Failed", "Assets/Failed.png", renderer);
    TextureManager::LoadTexture("GameOver", "Assets/GameOver.png", renderer);
    TextureManager::LoadTexture("Start", "Assets/Start.png", renderer);

    //LOAD SPRITES
    Sprite bulletSprite0;
    Sprite bulletSprite1;
    Sprite bulletSprite2;
    Sprite transition;
    Sprite shipStraight;
    Sprite enemyStraight;
    Sprite enemyZigzag;
    Sprite enemyCircle;
    Sprite life;
    Sprite boss;
    Sprite levelUp;
    Sprite failed;
    Sprite gameOver;
    Sprite start;

    for (int i = 0; i < 18; i++)
    {
        Sprite bgSprite;
        bgSprite.Load(TextureManager::GetTexture("BG_1"), Rectangle((i%8)*8,(i/8)*8,8,8), renderer);
        SpriteManager::AddSprite(string_format("BG_%d", i), bgSprite);
    }

    bulletSprite0.Load(TextureManager::GetTexture("Bullets"), Rectangle(0, 0, BULLET_WIDTH, BULLET_HEIGHT), renderer);
    bulletSprite1.Load(TextureManager::GetTexture("Bullets"), Rectangle(BULLET_WIDTH, 0, BULLET_WIDTH, BULLET_HEIGHT), renderer);
    bulletSprite2.Load(TextureManager::GetTexture("Bullets"), Rectangle(BULLET_WIDTH*2, 0, BULLET_WIDTH, BULLET_HEIGHT), renderer);
    transition.Load(TextureManager::GetTexture("Bullets"), Rectangle(0, 0, BULLET_WIDTH, BULLET_HEIGHT), renderer);
    shipStraight.Load(TextureManager::GetTexture("Ship"), Rectangle(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT), renderer);
    enemyStraight.Load(TextureManager::GetTexture("Enemies"), Rectangle(0, 0, ENEMY_WIDTH, ENEMY_HEIGHT), renderer);
    enemyZigzag.Load(TextureManager::GetTexture("Enemies"), Rectangle(ENEMY_WIDTH, 0, ENEMY_WIDTH, ENEMY_HEIGHT), renderer);
    enemyCircle.Load(TextureManager::GetTexture("Enemies"), Rectangle(2*ENEMY_WIDTH, 0, ENEMY_WIDTH, ENEMY_HEIGHT), renderer);
    life.Load(TextureManager::GetTexture("Life"), Rectangle(0, 0, 4, 4), renderer);
    boss.Load(TextureManager::GetTexture("Bosses"), Rectangle(0, 0, BOSS_WEIGHT, BOSS_HEIGHT), renderer);
    levelUp.Load(TextureManager::GetTexture("LevelUp"), Rectangle(0, 0, 64, 64), renderer);
    failed.Load(TextureManager::GetTexture("Failed"), Rectangle(0, 0, 64, 64), renderer);
    gameOver.Load(TextureManager::GetTexture("GameOver"), Rectangle(0, 0, 64, 64), renderer);
    start.Load(TextureManager::GetTexture("Start"), renderer);

    SpriteManager::AddSprite("Bullets_0", bulletSprite0);
    SpriteManager::AddSprite("Bullets_1", bulletSprite1);
    SpriteManager::AddSprite("Bullets_2", bulletSprite2);
    SpriteManager::AddSprite("Ship_Straight", shipStraight);
    SpriteManager::AddSprite("Enemy_Straight", enemyStraight);
    SpriteManager::AddSprite("Enemy_Zigzag", enemyZigzag);
    SpriteManager::AddSprite("Enemy_Circle", enemyCircle);
    SpriteManager::AddSprite("Life", life);
    SpriteManager::AddSprite("Boss_0", boss);
    SpriteManager::AddSprite("Level_Up", levelUp);
    SpriteManager::AddSprite("Failed", failed);
    SpriteManager::AddSprite("Game_Over", gameOver);
    SpriteManager::AddSprite("Start", start);
    SpriteManager::AddSprite("Transition", transition);

    //LOAD LEVELS
    LoadLevelFromFile("Assets/Level0.lvl");
    LoadLevelFromFile("Assets/Level1.lvl");

    //TODO: DEBUG DATA
    Logger::Log(Logger::Fatal, "Beta version, not complete yet!");
    Logger::Log(Logger::Fatal, "Starting at LEVEL 0 instead of LEVEL 5!");
    Logger::Log(Logger::Fatal, "Test Version 5!");
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
    this->mScreenTimer = 0.;
    this->mScreenToShot = 0;
    this->mStarted = false;
    this->mShooting = false;
    this->mControlX = 0;
    this->mControlY = 0;
    this->mControllerX = false;
    this->mControllerY = false;
    this->failedShown = false;
}

bool GameScene::Update(double delta)
{
    if (this->mScreenToShot == 0)
    {
        if (this->mStarted == true)
        {
            mScreenTimer += delta/16.f;
            if (this->mScreenTimer >= 10)
            {
                this->mScreenTimer = 0.0f;
                this->mScreenToShot = -1;
            }
        }
    }
    if (this->mScreenToShot == 1)
    {
        mScreenTimer += delta/16.f;
        if (this->mScreenTimer >= 50)
        {
            this->mScreenTimer = 0.0f;
            this->mScreenToShot = -1;
        }
    }
    else if (this->mScreenToShot == 1)
    {
        mScreenTimer += delta/16.f;
    }

    if (this->mShooting && this->mShootingTimer >= 25.f)
    {
        Shoot();
        this->mShootingTimer = 0.f;
    }
    this->mShootingTimer += (float)delta/16.f;

    this->mPlayer.Move(Vector2f(this->mControlX * 1.5f * (float)delta /16.f, this->mControlY * 1.5f * (float)delta /16.f));


    if (!this->mStarted)
    {
        return true;
    }


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
                HurtPlayer(10);
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
            this->mCurrentEnemy = 0;
            this->mTargetTimer = levels[this->mCurrentLevel].GetEnemySpawnInfo(this->mCurrentEnemy).GetStartTimer();
            this->mLevelStage = LevelStage::Level;
            this->mTimerStatus = 0;
            Vector2f startPosition(this->CurrentBoss.GetPosition());
            startPosition.X += BOSS_WEIGHT / 2 + ENEMY_WIDTH / 2;
            startPosition.Y += BOSS_HEIGHT / 2 + ENEMY_WIDTH / 2;
            for (float i = 0; i < 1.f; i+=0.1f)
            {
                for (float j = 0; j < MathHelper::TAUf; j+=MathHelper::TAUf/40.f)
                {
                    float x = cos(j+(i*MathHelper::TAUf));
                    float y = -sin(j+(i*MathHelper::TAUf));
                    float xi = ((BOSS_WEIGHT / 2 * i) - BOSS_WEIGHT/2) * x;
                    float yi = ((BOSS_HEIGHT / 2 * i)- BOSS_WEIGHT/2) * y;
                    Vector2f newSpawnPosition(startPosition.X + xi,startPosition.Y + yi);
                    int type = rand()%3;
                    Enemy enemy(static_cast<Bullet::Type>(type), newSpawnPosition, Vector2f(-(x+i*2), -(y+i*2)));
                    enemy.Destroy(2);
                    this->mEnemies.push_back(enemy);
                }
            }
            Logger::Log(Logger::Fatal, "Moving to Stage 1 - Level.");
            this->HealPlayer(50);
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
            this->mScreenTimer = 0;
            this->mLevelStage = LevelStage::Transition;
            bool clear = true;
            for (auto& enemy : this->mEnemies)
            {
                clear &= (enemy.IsDestroyed() == 1);
            }
            if (clear) 
            {
                this->mScreenToShot = 6;
                if (this->mCurrentLevel < 10)
                {
                    this->mNextLevel++;
                }
            }
            else
            {
                this->mScreenToShot = 5;
                
                this->HurtPlayer(50);
                if (this->mCurrentLevel > 0)
                {
                    this->mNextLevel--;
                }
            }
        }
    }
    this->mTimerStatus += delta/16.;
    
    for (int i = (int)this->mEnemies.size()-1; i >= 0; i--)
    {
        if (this->mEnemies[i].IsDestroyed() == 0 || this->mEnemies[i].GetAnimating() > 0)
        {
            this->mEnemies[i].Update(delta);
        }
        if (this->mEnemies[i].IsDestroyed() == 0)
        {

            Rectangle playerRectangle = this->mPlayer.GetRectangle();
            if (this->mEnemies[i].GetRectangle().Intersects(playerRectangle))
            {
                this->mEnemies[i].Destroy(1);
                this->HurtPlayer(10);
                continue;
            }
            
            if (this->mEnemies[i].TimerCheck()) 
            {
                this->mBullets.push_back(this->mEnemies[i].SpawnBullet());
            }
        }
        if (this->mEnemies[i].IsDestroyed() == 2 && !this->failedShown)
        {
            this->failedShown = true;
            this->mScreenToShot = 1;
            this->mScreenTimer = 0.f;
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
    this->mScreenTimer += (float)(delta/16.);
    if (this->mScreenTimer >= 10)
    {
        this->mCurrentLevel = this->mNextLevel;
    }
    if (this->mScreenTimer > 20)
    {
        this->mLevelStage = LevelStage::Bossfight;
        this->mScreenTimer = 0.f;
        this->mScreenToShot = -1;
        this->mTimerStatus = 0;
        this->mTargetTimer = 0;
        this->mEnemies.clear();
        this->failedShown = false;
        this->CurrentBoss.Recycle(levels[this->mCurrentLevel].GetBossId());
    }
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
            int tile = levels[this->mCurrentLevel].GetBackground()*3 + this->mBgTiles[(8-y)*8+x];
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

    if (this->mScreenToShot == 0)
    {
        destination.X = 0;
        destination.Y = 0;
        destination.Width = 64;
        destination.Height = 64;
        Sprite image = SpriteManager::GetSprite("Start");
        Colorf tempColor(1.f,1.f,1.f,1.f-(float)(this->mScreenTimer)/10.f);
        image.SetColorMod(tempColor);
        renderer.DrawSprite(image, destination);;
    }
    else if (this->mScreenToShot == 1)
    {
        destination.X = 0;
        destination.Y = 0;
        destination.Width = 64;
        destination.Height = 64;
        Sprite image = SpriteManager::GetSprite("Failed");
        Colorf tempColor(1.f,1.f,1.f,1.f-(float)(this->mScreenTimer)/90.f);
        image.SetColorMod(tempColor);
        renderer.DrawSprite(image, destination);;
    }
    else if (this->mScreenToShot == 5)
    {
        destination.X = 0;
        destination.Y = 0;
        destination.Width = 64;
        destination.Height = 64;
        Sprite image = SpriteManager::GetSprite("Transition");
        float timer = (float)(this->mScreenTimer)/10.f;
        if (timer > 1.f) timer = 2.f - timer;
        Colorf tempColor(2.f,2.f,2.f,timer);
        image.SetColorMod(tempColor);
        renderer.DrawSprite(image, destination);;
        image = SpriteManager::GetSprite("Failed");
        tempColor = Colorf(1.f,1.f,1.f,timer);
        image.SetColorMod(tempColor);
        renderer.DrawSprite(image, destination);
    }
    else if (this->mScreenToShot == 6) //DEAR LORD THIS WAS KILLING MY EMSCRIPTEN BUILD AND I HAVE NO IDEA HOW I FIXED THIS CASE. DON'T TOUCH IT!
    {
        destination.X = 0;
        destination.Y = 0;
        destination.Width = 64;
        destination.Height = 64;
        Sprite image = SpriteManager::GetSprite("Transition");
        float timer = (float)(this->mScreenTimer)/10.f;
        if (timer > 1.f) timer = 2.f - timer;
        Colorf tempColor(2.f,2.f,2.f,timer);
        image.SetColorMod(tempColor);
        renderer.DrawSprite(image, destination);;
        image = SpriteManager::GetSprite("Level_Up");
        tempColor = Colorf(1.f,1.f,1.f,timer);
        image.SetColorMod(tempColor);
        renderer.DrawSprite(image, destination);
    }
}




void GameScene::Shoot()
{
    this->mBullets.push_back(this->mPlayer.SpawnBullet());
    this->mStarted = true;
}

bool GameScene::HealPlayer(float amount)
{
    float hp = this->mPlayer.GetHealth();
    hp += amount;
    if (hp > 100.f)
    {
        hp -= 100.f;
        if (this->mLives < 5)
        {
            this->mLives++;
        }
        this->mVisualPlayerHealth = (double)hp;
        this->mPlayer.SetHealth(hp);
        return true;
    }

    this->mPlayer.SetHealth(hp);
    return false;
}

bool GameScene::HurtPlayer(float amount)
{
    float hp = this->mPlayer.GetHealth();
    hp -= amount;
    if (hp < 0.f)
    {
        hp += 100.f;
        Logger::Log(Logger::Fatal, "YOU DIED");
        Logger::Log(Logger::Fatal, "YOU MAY COLLECT YOUR SOULS ON PLACE YOU DIED AFTER YOU RESPAWN ON BONFIRE");
        Logger::Log(Logger::Fatal, "wait... that is darksouls");
        this->mLives--;
        if (this->mLives < 0)
        {
            hp = 100.f;
            this->mLives=3;
            Logger::Log(Logger::Fatal, "Lies don't really work now. RESET TO 3");
            //TODO: Do Game Over
        }
        this->mPlayer.SetHealth(hp);
        return true;
    }

    this->mPlayer.SetHealth(hp);
    return false;
}


void GameScene::KeyDown(SDL_KeyboardEvent e)
{
    switch (e.keysym.scancode)
    {
        case SDL_SCANCODE_ESCAPE:
            break;
        case SDL_SCANCODE_RETURN:
            break;  
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
            this->mControlY += 1;
            this->mControllerX = false;
            this->mControllerY = false;
            break;  
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
            this->mControlY += -1;
            this->mControllerX = false;
            this->mControllerY = false;
            break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
            this->mControlX += -1;
            this->mControllerX = false;
            this->mControllerY = false;
            break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
            this->mControlX += 1;
            this->mControllerX = false;
            this->mControllerY = false;
            break;
        case SDL_SCANCODE_SPACE:
            this->mShooting = true;
            this->mControllerX = false;
            this->mControllerY = false;
            this->mShootingTimer = 100.f;
            break;
        default:
            break;
    }
}

void GameScene::KeyUp(SDL_KeyboardEvent e)
{
    switch (e.keysym.scancode)
    {
        case SDL_SCANCODE_ESCAPE:
            break;
        case SDL_SCANCODE_RETURN:
            break;  
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
            this->mControlY -= 1;
            break;  
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
            this->mControlY -= -1;
            break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
            this->mControlX -= -1;
            break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
            this->mControlX -= 1;
            break;
        case SDL_SCANCODE_SPACE:
            this->mShooting = false;
            break;
        default:
            break;
    }
}

void GameScene::MouseMove(SDL_MouseMotionEvent e)
{
    this->mPlayer.Move(Vector2f(e.xrel*0.1f, -e.yrel*0.1f));
}

void GameScene::MouseButtonDown(SDL_MouseButtonEvent e)
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        this->mShooting = true;
        this->mShootingTimer = 100.f;
    }
}

void GameScene::MouseButtonUp(SDL_MouseButtonEvent e)
{
    this->mShooting = false;
}

void GameScene::ControllerAxisMove(SDL_ControllerAxisEvent e)
{
    if (e.axis == SDL_CONTROLLER_AXIS_LEFTX) 
    {
        if (e.value > -DEATH_ZONE && e.value < DEATH_ZONE)
        {
            if (this->mControllerX)
            {
                this->mControlX = 0;
                this->mControllerX = false;
            }
        }
        if (e.value < -DEATH_ZONE) 
        {
            this->mControlX = ((float)e.value) / 32767;
            this->mControllerX = true;
        }
        else if (e.value > DEATH_ZONE)
        {
            this->mControlX = ((float)e.value) / 32767;
            this->mControllerX = true;
        }
    }
    else if (e.axis == SDL_CONTROLLER_AXIS_LEFTY)
    {
        if (e.value > -DEATH_ZONE && e.value < DEATH_ZONE)
        {
            if (this->mControllerY)
            {
                this->mControlY = 0;
                this->mControllerY = false;
            }
        }
        if (e.value < -DEATH_ZONE)
        {
            this->mControlY = -((float)e.value) / 32767;
            this->mControllerY = true;
        }
        else if (e.value > DEATH_ZONE)
        {
            this->mControlY = -((float)e.value) / 32767;
            this->mControllerY = true;
        }
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
            this->mControlY += 1.f;
            this->mControllerX = false;
            this->mControllerY = false;
            break;  
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            this->mControlY += -1.f;
            this->mControllerX = false;
            this->mControllerY = false;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            this->mControlX += -1.f;
            this->mControllerX = false;
            this->mControllerY = false;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            this->mControlX += 1.f;
            this->mControllerX = false;
            this->mControllerY = false;
            break;
        case SDL_CONTROLLER_BUTTON_A:
            this->mShooting = true;
            this->mShootingTimer = 100.f;
            break;
        default:
            break;
    }
}

void GameScene::ControllerButtonUp(SDL_ControllerButtonEvent e)
{    
    switch (e.button)
    {
        case SDL_CONTROLLER_BUTTON_BACK:
            break;
        case SDL_CONTROLLER_BUTTON_START:
            break;  
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            this->mControlY -= 1.f;
            break;  
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            this->mControlY -= -1.f;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            this->mControlX -= -1.f;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            this->mControlX -= 1.f;
            break;
        case SDL_CONTROLLER_BUTTON_A:
            this->mShooting = false;
            break;
        default:
            break;
    }
}