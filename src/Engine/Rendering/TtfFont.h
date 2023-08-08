#ifdef USE_SDL2D
    #include "2D/TtfFont.h"
#else
    #ifdef USE_OPENGL
        #include "OpenGL/TtfFont.h"
    #else
        #ifdef USE_OPENGL_ES
            #include "OpenGL/TtfFont.h"
        #endif
    #endif
#endif