#ifdef USE_SDL2D
    #include "2D/Shader.h"
#else
    #ifdef USE_OPENGL
        #include "OpenGL/Shader.h"
    #else
        #ifdef USE_OPENGL_ES
            #include "OpenGL/Shader.h"
        #endif
    #endif
#endif