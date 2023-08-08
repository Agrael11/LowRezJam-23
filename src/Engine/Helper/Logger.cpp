#ifndef EMSCRIPTEN
#include <fstream>
#else
#include <emscripten.h>
#endif

#include "Logger.h"
#include "format.h"

using namespace Engine::Helper;

Logger::Level Logger::MinimumPrintLevel = Logger::Info;
Logger::Level Logger::MinimumFileLevel = Logger::Warning;
bool Logger::PrintToFile = false;
bool Logger::OverrideFile = true;
bool Logger::OpenedFile = false;
bool Logger::SaveTimedCopy = true;
std::string Logger::FileName = "";
std::string Logger::TimedCopyName = "";


const std::string return_current_time_and_date(std::string format)
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    #if WIN32
        localtime_s(&tstruct, &now);
    #else
        localtime_r(&now, &tstruct);
    #endif
    strftime(buf, sizeof(buf), format.c_str(), &tstruct);
    return buf;
}

std::string Logger::MakeColor(Logger::ConsoleColor color, bool background, bool bright)
{
    #if EMSCRIPTEN

    return "";

    #else

    int col = color;

    if (background) col += 40;
    else col += 30;
    if (bright) col += 60;

    if (col == -1) col = 0;

    return string_format("\x1b[%dm", col);

    #endif
}

void Logger::Log(Logger::Level level, std::string message)
{   
    if (level < MinimumPrintLevel) return;

    std::string realTime = return_current_time_and_date("%Y-%m-%d %X");

    std::string printText;

    switch (level)
    {
        case Debug:
        printf("[%sDebug%s @ %s%s%s] %s\n", MakeColor(White, false, true).c_str(), MakeColor(White, false, false).c_str(), MakeColor(Blue, false, true).c_str(), realTime.c_str(), MakeColor(White, false, false).c_str(), message.c_str());
        printText = string_format("[Debug @ %s] %s\n", realTime.c_str(),  message.c_str());
        break;
        case Info:
        printf("[%sInfo%s @ %s%s%s] %s\n", MakeColor(Blue, false, true).c_str(), MakeColor(White, false, false).c_str(), MakeColor(Blue, false, true).c_str(), realTime.c_str(), MakeColor(White, false, false).c_str(), message.c_str());
        printText = string_format("[Info @ %s] %s\n", realTime.c_str(),  message.c_str());
        break;
        case Warning:
        printf("[%sWarning%s @ %s%s%s] %s\n", MakeColor(Yellow, false, true).c_str(), MakeColor(White, false, false).c_str(), MakeColor(Blue, false, true).c_str(), realTime.c_str(), MakeColor(White, false, false).c_str(), message.c_str());
        printText = string_format("[Warning @ %s] %s\n", realTime.c_str(),  message.c_str());
        break;
        case Error:
        printf("[%sError%s @ %s%s%s] %s\n", MakeColor(Red, false, true).c_str(), MakeColor(White, false, false).c_str(), MakeColor(Blue, false, true).c_str(), realTime.c_str(), MakeColor(White, false, false).c_str(), message.c_str());
        printText = string_format("[Error @ %s] %s\n", realTime.c_str(),  message.c_str());
        break;
        default:
        case Fatal:
        printf("[%sFatal%s @ %s%s%s] %s\n", MakeColor(Red, false, false).c_str(), MakeColor(White, false, false).c_str(), MakeColor(Blue, false, true).c_str(), realTime.c_str(), MakeColor(White, false, false).c_str(), message.c_str());
        printText = string_format("[Fatal @ %s] %s\n", realTime.c_str(),  message.c_str());
        break;
    }
    
    #ifndef EMSCRIPTEN
    if (PrintToFile)
    {
        std::string append = "";
        
        if (OpenedFile || !OverrideFile)
        {
            std::ifstream myInFile (FileName);
            append = std::string((std::istreambuf_iterator<char>(myInFile)), std::istreambuf_iterator<char>());
            myInFile.close();
        }

        std::ofstream myOutFile (FileName);
        myOutFile << append;
        myOutFile << printText;
        myOutFile.close();

        if (SaveTimedCopy)
        {
            if (TimedCopyName == "")
            {
                TimedCopyName = string_format("%s_%s_%s",return_current_time_and_date("%Y-%m-%d"), return_current_time_and_date("%H-%M-%S"),FileName);
            }
            std::ofstream mySecondOutFile (TimedCopyName);
            mySecondOutFile << append;
            mySecondOutFile << printText;
            mySecondOutFile.close();
        }

        OpenedFile = true;
    }
    #endif
}