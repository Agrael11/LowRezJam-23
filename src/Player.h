#pragma once

#include "Engine/Rendering/Sprite.h"
#include "Engine/Rendering/ImageTexture.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Math/Vector2f.h"
#include "Bullet.h"

#define PLAYER_WIDTH 4
#define PLAYER_HEIGHT 8

class Player
{
    private:
        Engine::Math::Vector2f mPosition;

        float mHealth;

    public:
        Player();

        void Draw(Engine::Rendering::Renderer &renderer);
        void Update(double deltaTime);
        void Move(Engine::Math::Vector2f target);
        Bullet SpawnBullet();
        Engine::Math::Vector2f GetPosition();
        Engine::Math::Rectangle GetRectangle();
        float GetHealth();
        void SetHealth(float health);
        float Hurt(float amount);
};
