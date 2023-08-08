#include "Player.h"

#include "Engine/Math/Rectangle.h"
#include "Engine/Rendering/ImageTexture.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Engine/Rendering/Sprite.h"

using namespace Engine::Math;

Player::Player()
{
    this->mPosition = Vector2f((64-PLAYER_WIDTH)/2.f, (float)PLAYER_HEIGHT);
    this->mHealth = 100.f;
    this->mVisualHealth = 100.f;
    this->mVisualHealthSpeed = 1.f;
}

void Player::Draw(Engine::Rendering::Renderer &renderer)
{
    Rectangle dest = Rectangle((int)this->mPosition.X, (int)this->mPosition.Y, PLAYER_WIDTH, PLAYER_HEIGHT);
    renderer.DrawSprite(SpriteManager::GetSprite("Ship_Straight") , dest);
    dest = Rectangle(0, 0, (int)(64*this->mVisualHealth/100.f), 2);
    renderer.DrawSprite(SpriteManager::GetSprite((this->mHealth > 66.f)?"Bullets_2":((this->mHealth > 33.f)?"Bullets_0":"Bullets_1")) , dest);
}

void Player::Update(double deltaTime)
{
    if (this->mPosition.X < 0) this->mPosition.X = 0;
    if (this->mPosition.X > 64 - PLAYER_WIDTH) this->mPosition.X = 64 - PLAYER_WIDTH;
    if (this->mPosition.Y < 0) this->mPosition.Y = 0;
    if (this->mPosition.Y > 64 - PLAYER_HEIGHT) this->mPosition.Y = 64 - PLAYER_HEIGHT;
    float difference = this->mVisualHealth - this->mHealth;
    if (abs(difference) < this->mVisualHealthSpeed)
    {
        this->mVisualHealth = this->mHealth;
    }
    else if (difference < 0)
    {
        this->mVisualHealth += mVisualHealthSpeed * (float)deltaTime / 16.f * 2;
    }
    else if (difference > 0)
    {
        this->mVisualHealth -= mVisualHealthSpeed * (float)deltaTime / 16.f * 2;
    }
}

void Player::Move(Vector2f target)
{
    this->mPosition.X += target.X;
    this->mPosition.Y += target.Y;
}

Vector2f Player::GetPosition()
{
    return this->mPosition;
}

Bullet Player::SpawnBullet()
{
    Vector2f position = Vector2f(this->mPosition.X, this->mPosition.Y);
    position.X += 1;
    position.Y += PLAYER_HEIGHT;
    return Bullet(position, Vector2f(0,1), true, Bullet::Type::Static);
}

Rectangle Player::GetRectangle()
{
    return Rectangle((int)this->mPosition.X, (int)this->mPosition.Y, PLAYER_WIDTH, PLAYER_HEIGHT);
}

int Player::GetHealth()
{
    return (int)this->mHealth;
}

void Player::SetHealth(int health)
{
    this->mHealth = (float)health;
    if (this->mHealth > 100) this->mHealth = 100;
    if (this->mHealth < 0) this->mHealth = 0;
}

int Player::Hurt(int amount)
{
    this->SetHealth((int)(this->mHealth - amount));
    return (int)this->mHealth;
}