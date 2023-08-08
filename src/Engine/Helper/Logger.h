#pragma once

#include <iostream>

namespace Engine::Helper::Logger
{
    enum Level {Debug, Info, Warning, Error, Fatal};
    enum ConsoleColor {Black, Red, Green, Yellow, Blue, Magenta, Cyan, White, Reset = -1};
    
    extern std::string FileName;
    extern bool PrintToFile;
    extern bool OverrideFile;
    extern bool OpenedFile;
    extern bool SaveTimedCopy;
    extern std::string TimedCopyName;
    extern Level MinimumPrintLevel;
    extern Level MinimumFileLevel;
    
    std::string MakeColor(ConsoleColor color, bool background, bool bright);
    void Log(Level level, std::string message);
};