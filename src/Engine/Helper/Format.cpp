#include "Format.h"


std::vector<std::string> Engine::Helper::splitString(std::string input, char splitChar)
{
    std::istringstream iss(input);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(iss, token, splitChar)) {
        tokens.push_back(token);
    }

    return tokens;
}