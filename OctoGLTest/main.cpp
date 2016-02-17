#include "DefaultShaders.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>

SDL_Window * _window;
void * _rawGLContext;

GLuint vbo = 0;
GLuint vao = 0;
GLuint shader_program = 0;

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

bool InitVertexBufferObject()
{
    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };
    
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), points, GL_STATIC_DRAW);
    
    return true;
}

bool InitVertexArrayObject()
{
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    return true;
}

bool InitShader()
{
    GLuint vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);
    
    GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);
    
    shader_program = glCreateProgram ();
    glAttachShader (shader_program, fs);
    glAttachShader (shader_program, vs);
    glLinkProgram (shader_program);
    
    return true;
}

bool quit = false;

int main(int argc, const char * argv[])
{
    // Initialize everything
    if (!InitSDL()) return 1;
    if (!OpenWindow("OctoGL v0.0.0", 640, 480, false)) return 1;
    if (!InitGLEW()) return 1;
    
    if (!InitVertexBufferObject()) return 1;
    if (!InitVertexArrayObject()) return 1;
    if (!InitShader()) return 1;
    
    // Main application loop
    SDL_Event event;
    while (!quit) {
        // First wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Set our OpenGL state
        glUseProgram (shader_program);
        glBindVertexArray (vao);
        // Draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays (GL_TRIANGLES, 0, 3);
        
        // Update events like input handling
        if (SDL_WaitEvent(&event) >= 0) {
            switch (event.type) {
                case SDL_QUIT: {
                    printf("Quit requested, so I'm quitting.\n");
                    quit = true;
                }
                case SDL_KEYUP: {
                    std::string key = SDL_GetKeyName(event.key.keysym.sym);
                    if (key == "Escape") {
                        printf("Escape pressed, so I'm quitting.\n");
                        quit = true;
                    }
                }
                    break;
            }
        }
        // Swap buffers, this actually puts the stuff we drew before on the screen
        SDL_GL_SwapWindow(_window);
    }
    
    // Don't forget to clean up :)
    SDL_Quit();
    
    return 0;
}
