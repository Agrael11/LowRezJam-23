#include "Shader.h"

#include "../../Helper/Logger.h"
#include "../../Helper/Format.h"

using namespace Engine::Rendering;
using namespace Engine::Helper;

unsigned int Shader::GetHandle()
{
    return this->mShaderProgram;
}

bool Shader::LoadFromStrings(std::string vertexString, std::string fragmentString)
{
    Logger::Log(Logger::Warning, "Shaders not supported in SDL2D");

    return true;
}

bool Shader::LoadFromFile(std::string vertexFilePath, std::string fragmentFilePath)
{
    Logger::Log(Logger::Warning, "Shaders not supported in SDL2D");

    return true;
}

void Shader::UseShader()
{
    Logger::Log(Logger::Warning, "Shaders not supported in SDL2D");
}