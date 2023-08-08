#include "Shader.h"

#include "glad/glad.h"
#include "../../Helper/Logger.h"
#include "../../Helper/Format.h"

#include <fstream>

using namespace Engine::Rendering;
using namespace Engine::Helper;

unsigned int Shader::GetHandle()
{
    return this->mShaderProgram;
}

bool Shader::LoadFromStrings(std::string vertexString, std::string fragmentString)
{
    int  success;
    char infoLog[512];
    
    //Compiling vertex shader

    const char* vertexShaderSource = vertexString.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

        Logger::Log(Logger::Error, string_format("Failed to compile vertex shader: %s", infoLog));
        return false;
    }

    //Compiling fragment shader

    const char* fragmentShaderString = fragmentString.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

        Logger::Log(Logger::Error, string_format("Failed to compile fragment shader: %s", infoLog));
        return false;
    }

    //Linking shader program

    this->mShaderProgram = glCreateProgram();

    glAttachShader(this->mShaderProgram, vertexShader);
    glAttachShader(this->mShaderProgram, fragmentShader);
    glLinkProgram(this->mShaderProgram);

    glGetProgramiv(this->mShaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->mShaderProgram, 512, NULL, infoLog);

        Logger::Log(Logger::Error, string_format("Failed to link shader program: %s", infoLog));
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool Shader::LoadFromFile(std::string vertexFilePath, std::string fragmentFilePath)
{
    std::ifstream myInFile (vertexFilePath);
    if (myInFile.fail())
    {
        Logger::Log(Logger::Error, string_format("Failed to open vertex shader file %s.", vertexFilePath.c_str()));
    }
    std::string vertexData = std::string((std::istreambuf_iterator<char>(myInFile)), std::istreambuf_iterator<char>());
    myInFile.close();

    myInFile.open(fragmentFilePath);
    if (myInFile.fail())
    {
        Logger::Log(Logger::Error, string_format("Failed to open fragment shader file %s.", fragmentFilePath.c_str()));
        return false;
    }
    std::string fragmentData = std::string((std::istreambuf_iterator<char>(myInFile)), std::istreambuf_iterator<char>());
    myInFile.close();

    return this->LoadFromStrings(vertexData, fragmentData);
}

void Shader::UseShader()
{
    glUseProgram(this->mShaderProgram);
}