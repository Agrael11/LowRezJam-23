#pragma once

#include <iostream>

namespace Engine::Rendering
{
    class Shader
    {
        private:
        unsigned int mShaderProgram;
        public:

        unsigned int GetHandle();

        bool LoadFromStrings(std::string vertexString, std::string fragmentString);

        bool LoadFromFile(std::string vertexFilePath, std::string fragmentFilePath);

        void UseShader();
    };
};