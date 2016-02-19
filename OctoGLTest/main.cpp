#include "AppContext.h"
#include "GLBackend.h"

#include "DefaultShaders.h"
#include <SDL2/SDL.h>
#include <string>

VertexBufferID vbID = 0;
VertexLayoutID vlID = 0;
ShaderID shaderID = 0;
GLBackend backend;

bool InitVertexBufferObject()
{
    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };
    
    vbID = backend.addVertexBuffer(9*sizeof(float),
                                   STATIC,
                                   points);
    return true;
}

bool InitVertexArrayObject()
{
    VertexFormatDesc desc = {
        0, FLOAT, 3
    };
    
    vlID = backend.addVertexLayout(1, &desc, &vbID);
    return true;
}

bool InitShader()
{
    shaderID = backend.addShader(fragment_shader, vertex_shader);
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
        float clearColor[4] = { 0.3, 0.3, 0.3, 0.0 };
        backend.clear(true, true, false, clearColor, 0.0, 0);
        // Set our render state
        backend.setShader(shaderID);
        backend.setVertexLayout(vlID);
        // Draw points 0-3 from the currently bound vertices with current shader
        backend.drawArrays(DRAW_TRIANGLES, 0, 3);
        
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
