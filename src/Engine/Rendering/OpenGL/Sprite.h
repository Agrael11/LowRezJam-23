#pragma once

#include <iostream>

#include "../../Math/Rectangle.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector2f.h"
#include "../../Math/Colorf.h"
#include "Texture.h"
#include "Shader.h"

namespace Engine::Rendering::BlendMode
{
    enum BlendMode {Add, Subtract, ReverseSubtract, Min, Max};
}

namespace Engine::Rendering
{
    class Renderer;
    class Sprite
    {
    private:
        Texture mTexture;
        Engine::Math::Vector2 mSize;
        Engine::Math::Colorf mColor;
        
        unsigned int mVAO;
        unsigned int mVBO;
        unsigned int mEBO;
        bool mVAOCreated;

        void mBuildVAO();

    public:
        Engine::Math::Rectangle sourceRectangle;
        Engine::Math::Vector2f origin;

        bool Load(Texture texture, Renderer &renderer);
        bool Load(Texture texture, Engine::Math::Rectangle sourceRectangle, Renderer &renderer);
        Texture* GetTexture();
        void SetTexture(Texture texture);
        void SetTexture(Texture texture, Engine::Math::Rectangle sourceRectangle);
        void SetOrigin(float x, float y);
        void SetSourceRectangle(Engine::Math::Rectangle &r);
        int GetWidth();
        int GetHeight();
        float GetRatio();
        Engine::Math::Vector2* GetSize();
        bool Draw(Engine::Math::Rectangle &destinationRectangle, Renderer &renderer, double rotationRad = 0, bool flipHorizontal = false, bool flipVertical = false);
        bool Draw(Engine::Math::Rectangle &sourceRectangle, Engine::Math::Rectangle &destinationRectangle, Renderer &renderer, double rotationRad = 0, bool flipHorizontal = false, bool flipVertical = false);
        bool SetBlendMode(Engine::Rendering::BlendMode::BlendMode mode);
        bool SetColorMod(Engine::Math::Colorf &color);
        void Unload();
    };
};