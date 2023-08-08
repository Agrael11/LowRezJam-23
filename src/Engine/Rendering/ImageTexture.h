#ifdef USE_SDL2D
    #include "2D/ImageTexture.h"
#else
    #ifdef USE_OPENGL
        #include "OpenGL/ImageTexture.h"
    #else
        #ifdef USE_OPENGL_ES
            #include "OpenGL/ImageTexture.h"
        #endif
    #endif
#endif