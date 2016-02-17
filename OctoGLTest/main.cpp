#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>

SDL_Window * _window;
void * _rawGLContext;

bool InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return false;

    //Use OpenGL 3.2 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    return true;
}

bool OpenWindow(const std::string title,
                const int width,
                const int height,
                const bool fullScreen) {
    
    Uint32 flags = SDL_WINDOW_OPENGL;
    if (fullScreen) flags |= SDL_WINDOW_FULLSCREEN;
    
    // Create a window
    _window = SDL_CreateWindow(title.c_str(),          // window title
                               SDL_WINDOWPOS_CENTERED,  // x position, centered
                               SDL_WINDOWPOS_CENTERED,  // y position, centered
                               width,                   // width, in pixels
                               height,                  // height, in pixels
                               SDL_WINDOW_OPENGL);      // flags
    
    //Create context
    _rawGLContext = SDL_GL_CreateContext( _window );
    if( _rawGLContext == NULL ) {
        printf("OpenGL context could not be created!\n");
        printf("SDL Error: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool InitGLEW()
{
    //Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        printf("Error initializing GLEW! %s",
               glewGetErrorString(glewError));
        return false;
    }
    glGetError();   // Ignore one bad ENUM in GLEW initialization
    
    const GLubyte * version = glGetString(GL_VERSION);
    printf("OpenGL Version:%s\n", version);
    
    GLint major = 0;
    GLint minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    if (major < 3 || (major == 3 && minor < 2))
    {
        printf("Error could not get a modern OpenGL context!");
    }
    
    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
    {
        printf("Unable to set VSync! SDL Error: %s",
               SDL_GetError());
    }
    
    printf("Created OpenGL context.\n");
    printf("Pixel format: %s\n",
    SDL_GetPixelFormatName(SDL_GetWindowPixelFormat(_window)));

    return true;
}

int main(int argc, const char * argv[])
{

    if (!InitSDL()) return 1;
    if (!OpenWindow("OctoGL v0.0.0", 640, 480, false)) return 1;
    if (!InitGLEW()) return 1;
    
    return 0;
}
