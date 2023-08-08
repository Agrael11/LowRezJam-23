# SDL2 Render Engine Base

This is simple SDL2 based engine, that I want to use to make some simple games

## Supported build targets

Engine supports building for Emscripten, Linux a Windows

## Features

Handles : 
- Audio playback
- Sprites & Rendertextures rendering
- Controls using mouse, keyboard and controller

## Dependencies

### Windows

Requires Visual Studio 2022 for building (may work with older version) with C++ and CMake installed  
Recommending to use VSCode for editing  
Uses *vcpkg* to handle packages (https://github.com/Microsoft/vcpkg)  
Vcpkg packages - *SDL2*, *SDL2-image*, *SDL2-mixer*  

### Linux

Recommending GCC at least **9.4.0**  
Use your prefered way to download *libsdl2-dev*, *libsdl2-image-dev* and *libsdl2-mixer-dev* (or alternatives for your distro)  

### Emscripten

I used linux version of emscripten to build, however feel free to use windows one if it will work for you. **Just don't forget to update path in CMakePresets.json**  

## Usage

### Logger:

**Namespace : Engine::Helper**  

Loggers default settings are set on the start of main function, in the Main.cpp.  
I recommend configuring them to your liking.  
- *Logger::MinimumPrintLevel* (Logger::Level) sets the minimum level of message printed into console
- *Logger::MinimumFileLevel* (Logger::Level) sets the minimum level of message prinetd into log file
- *Logger::FileName* (std::string) sets the name of your log file
- *Logger::PrintToFile* (bool) sets, whether log file will be used (true = use file)
- *Logger::Overridefile* (bool) sets, wheter log file will be overriden on start, or appended (true = override)
- *Logger::SaveTimedCopy* (bool) sets, wheter there will be copy of your log file, with timestamp of first logged message (true = use file)

Logger have 5 message levels, in following order: Debug, Info, Warning, Error and Fatal.  
You can use *Logger::Log(Logger::Level, std::string message)* function to log a message.  
If needed, I provided *string_format* function in *Engine/Helper/Format.h* file  

### Sprites

**Namespace : Engine::Rendering**

Sprite should be Loaded in LoadContent function of your game's class (Default provided: Game.cpp)  
You can use *Sprite->Load(std:string filePath, SDL_Renderer\* renderer)* or *Sprite->Load(std::string filePath, SDL_Rect sourceRectangle, SDL_Renderer\* renderer)* to load the sprite from file.  

You should Unload your sprite using *Unload* function in the Quit function of your game's class.  

*sourceRectangle* selects area of texture that will be used to draw. You can also get access to it directly in *Sprite->sourceRectangle*  

- *SetOrigin(double x, double y)* sets origin point for rotation, with values ranging from 0 to 1
- *SetBlendMode(SDL_BlendMode mode)* you can select blend mode of sprite
- *SetAlphaMod(int alpha)* lets you select additional alpha to your sprite
- *SetColorMod(int r, int g, int b)* lets you colorize your sprite
- *Draw(SDL_Rect\* destinationRectangle, SDL_Renderer\* renderer, [SDL_Rect\* source = NULL], [double rotation = 0], [SDL_RendererFlip flipping])* is used to draw sprite, but I recommend using Draw method in Renderer class
- *Draw(SDL_Rect\* sourceRectangle, SDL_Rect\* destinationRectangle, SDL_Renderer\* renderer, [SDL_Rect\* source = NULL], [double rotation = 0], [SDL_RendererFlip flipping])* is a variant with custom specified source rectangle.

*Note: You can get SDL_Renderer\* using GetSDL_Renderer function in Renderer class.*  

### Render Textures

**Namespace :: Engine::Rendering**

They work similar to sprites, but instead of Load function you have *Create(int width, int height, SDL_Renderer\* renderer)* function.  
To use them you can use *SetAsRenderTarget(SDL_Renderer\* renderer)*, but I recommend using SetRenderTarget in Renderer function for consistency.  

### Renderer

**namespace : Engine::Rendering**

Renderer is automatically initialized at the start of your Game function using *Load(int width, int height, std::string windowTitle)* function and saved into it's *renderer* variable. Renderer is also automatically destroyed using *Destroy()* function in the Unload() function Game class

You should call *Begin()* at the start of rendering and *End()* at the end of rendering in Draw function of your game class.  
If you're rendering into RenderTexture, you need to call these after setting active rendertextures and resetting default render target respectively.  

*Note: Begin() currently doesn't do anything.

You can draw Sprite and Render Texture using *DrawSprite(Sprite\* sprite, SDL_Rect\* destination, [SDL_Rect\* source = NULL], [double rotation = 0], [SDL_RendererFlip flipping])* or *DrawRenderTexture(RenderTexture\* renderTexture, SDL_Rect\* destination, [SDL_Rect\* source = NULL], [double rotation = 0], [SDL_RendererFlip flipping])* respectively.  
SDL_Rect\* destinatation is place where on screen will image be placed, with NULL being stretched to entire screen/render target.  

You can also you *Clean(int r, int g, int b, int a)* to fill the render target with selected color.  

With *SetViewport(int x, int y,  int width, int height)* you can select current viewport.  

To enable and disable active RenderTextures you can use *SetRenderTarget(RenderTexture\* texture)* and *CleanRenderTarget()* respectively.

You can get SDL_Renderer\* by calling *GetSDL_Renderer()* function

### Audio Setup

**namespace : Engine::Audio**

Audio should be initialized in Init() function of your Game's class. By default *InitAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);* is provided. No I have no idea how it works  

### Sound effect

**namespace : Engine::Audio**

To Load a Sound effect, use Sound classes *Load(std::string path)* function in your LoadContent function of game's class.
You should unload a Sound effect using *Unload()* in your Quit function.

To play a sound effect, you can use *Play(int loops)* or *Play(int loops, int volume)* functions.   
*loop* select how many times sound effect will loop, 0 being not looping and -1 will loop indefinitely.

To stop a sound effect, just use *Stop()* function.

You can check, if sound effect is playing using *IsPlaying()* function.

### Music

**namespace : Engine::Audio**

To Load a Music file, use Music classes *Load(std::string path)* function in your LoadContent function of game's class.  
You should unload a Music using *Unload()* in your Quit function.  

To play a music file, you can use *Play(int loops)* or *Play(int loops, bool override, int fadeout)* functions.  
*loop* select how many times sound effect will loop, 0 being not looping and -1 will loop indefinitely.  
*override* select, wheter currently playing music should stop.  
*fadout* will select slow fadeout for currently playing music, or 0 to stop immediately

To stop any playing music, just use *Stop(int fadeout)* function, with fadeout selecting slow fadout for playing music, or 0 to stop immediately.

You can check, if any music is playing using *IsPlaying()* function.  

To set volume use *SetVolume(int volume)* function.
You can also Pause, Rewind or Resume using *Pause()* *Rewind()* or *Resume()* functions.

### Controls

Controls are handled using standard SDL_Event in *HandleEvent* function of your game's class.  
By default a mapping into *KeyDown*, *KeyUp*, *MouseButtonDown*, *MouseButtonUp*, *MouseMove*, *MouseWheelMove*, *ControllerButtonDown*, *ControllerButtonUp* and *ControllerAxisMove* functions are premade for easy access.