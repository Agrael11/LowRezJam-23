#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include "Game.h"
#include "Engine/Helper/Logger.h"
#include "Engine/Helper/Format.h"
#include "Engine/Support.h"

using namespace Engine::Helper;

bool quit = false;
Game* game;

unsigned int a = SDL_GetTicks();
unsigned int b = SDL_GetTicks();
double delta = 0;
bool controllerChanged = false;

void CheckEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (Engine::Support::controller)
        {
            if (e.type == SDL_CONTROLLERDEVICEADDED)
            {
                int id = e.cdevice.which;
                SDL_GameController* controller = SDL_GameControllerOpen(id);
                if (controller == NULL)
                {
                    Logger::Log(Logger::Error, string_format("Error opening controller %d! SDL_Error: %s.", id, SDL_GetError()));
                }
                else
                {
                    Logger::Log(Logger::Debug, string_format("Opening controller %d (%s)", id, SDL_GameControllerName(controller)));
                }
                controllerChanged = true;
            }
            else if (e.type == SDL_CONTROLLERDEVICEREMOVED)
            {
                int id = e.cdevice.which;
                SDL_GameController* controller = SDL_GameControllerFromInstanceID(id);
                if (controller == NULL)
                {
                    Logger::Log(Logger::Error, string_format("Error closing controller %d! SDL_Error: %s.", id, SDL_GetError()));
                }
                else
                {
                    Logger::Log(Logger::Debug, string_format("Closing controller %d (%s)", id, SDL_GameControllerName(controller)));
                    SDL_GameControllerClose(controller);
                }
                controllerChanged = true;
            }
            else
            {
                game->HandleEvent(e);
            }
        }
        else
        {
            game->HandleEvent(e);
        }
    }
}

void Run()
{
    controllerChanged = false;

    a = SDL_GetTicks();
    delta = a - b;
    b = a;

    CheckEvents();
    int changed = 0;
    if (Engine::Support::controller)
    {
        while (controllerChanged)
        {
            SDL_GameControllerUpdate();
            changed++;
            Logger::Log(Logger::Debug, string_format("Rechecking events after controller state change (Repeat: %d)", changed));
            controllerChanged = false;
            CheckEvents();
        }
    }

    if (!game->Update(delta))
    {
        quit = true;
    }
    
    game->Draw(delta);
}

int main(int argc, char *args[])
{
    Logger::MinimumPrintLevel = Logger::Debug;
    Logger::MinimumFileLevel = Logger::Debug;
    Logger::FileName = "Log.log";
    Logger::PrintToFile = false;
    Logger::SaveTimedCopy = false;
    Logger::OverrideFile = true;

    Game tempGame = Game(512, 512, "LRJ_23");
        game = &tempGame;
        game->Init();
        game->LoadContent();

    #ifdef EMSCRIPTEN
        emscripten_set_main_loop(Run, 0, 1);
    #else
        while (quit == false)
        {
            Run();
        }
    #endif
        game->Unload();
    return 0;
}