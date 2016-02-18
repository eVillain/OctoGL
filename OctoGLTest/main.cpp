#include "AppContext.h"
#include "GLBackend.h"

#include "DefaultShaders.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>

VertexBufferID vbID = 0;
VertexLayoutID vlID = 0;
//GLuint vao = 0;
GLuint shader_program = 0;
GLBackend backend;

bool InitVertexBufferObject()
{
    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };
    
    // Setup VertexBuffer
    vbID = backend.addVertexBuffer(9*sizeof(float),
                                   STATIC,
                                   points);
    return true;
}

bool InitVertexArrayObject()
{
    // Setup VertexArray
    VertexFormatDesc desc = {
        0, FLOAT, 3
    };
    
    vlID = backend.addVertexLayout(1, &desc, &vbID);

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
        backend.setVertexLayout(vlID);
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
