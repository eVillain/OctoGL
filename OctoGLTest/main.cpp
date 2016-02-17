#include "AppContext.h"
#include "DefaultShaders.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>

GLuint vbo = 0;
GLuint vao = 0;
GLuint shader_program = 0;

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
    AppContext appContext;
    appContext.InitApp("OctoGL v0.0.0");
    
    if (!InitVertexBufferObject()) return 1;
    if (!InitVertexArrayObject()) return 1;
    if (!InitShader()) return 1;
    
    SDL_Event event;
    
    // Main application loop
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
                    printf("[Main] Quit requested, so I'm quitting.\n");
                    quit = true;
                }
                case SDL_KEYUP: {
                    std::string key = SDL_GetKeyName(event.key.keysym.sym);
                    if (key == "Escape") {
                        printf("[Main] Escape pressed, so I'm quitting.\n");
                        quit = true;
                    }
                }
                    break;
            }
        }
        // Swap buffers, this actually puts the stuff we drew before on the screen
        appContext.GetWindow()->SwapBuffers();
    }
    
    return 0;
}
