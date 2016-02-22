#include "AppContext.h"
#include "Renderer.h"
#include "RendererBackend.h"
#include "Commands.h"
#include "Camera.h"

#include <SDL2/SDL.h>
#include <string>


// Cube verts, position xyzw and normal xyzw
const float Cube_3D[] = {
    // front
    -0.5, -0.5,  0.5,  1.0,    0.0, 0.0, 1.0, 1.0,
    0.5, -0.5,  0.5,  1.0,    0.0, 0.0, 1.0, 1.0,
    0.5,  0.5,  0.5,  1.0,    0.0, 0.0, 1.0, 1.0,
    0.5,  0.5,  0.5,  1.0,    0.0, 0.0, 1.0, 1.0,
    -0.5,  0.5,  0.5,  1.0,    0.0, 0.0, 1.0, 1.0,
    -0.5, -0.5,  0.5,  1.0,    0.0, 0.0, 1.0, 1.0,
    // right
    0.5, -0.5,  0.5,  1.0,    1.0, 0.0, 0.0, 1.0,
    0.5, -0.5, -0.5,  1.0,    1.0, 0.0, 0.0, 1.0,
    0.5,  0.5, -0.5,  1.0,    1.0, 0.0, 0.0, 1.0,
    0.5,  0.5, -0.5,  1.0,    1.0, 0.0, 0.0, 1.0,
    0.5,  0.5,  0.5,  1.0,    1.0, 0.0, 0.0, 1.0,
    0.5, -0.5,  0.5,  1.0,    1.0, 0.0, 0.0, 1.0,
    // back
    -0.5,  0.5, -0.5,  1.0,    0.0, 0.0, -1.0, 1.0,
    0.5,  0.5, -0.5,  1.0,    0.0, 0.0, -1.0, 1.0,
    0.5, -0.5, -0.5,  1.0,    0.0, 0.0, -1.0, 1.0,
    0.5, -0.5, -0.5,  1.0,    0.0, 0.0, -1.0, 1.0,
    -0.5, -0.5, -0.5,  1.0,    0.0, 0.0, -1.0, 1.0,
    -0.5,  0.5, -0.5,  1.0,    0.0, 0.0, -1.0, 1.0,
    // left
    -0.5, -0.5, -0.5,  1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5, -0.5,  0.5,  1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5,  0.5,  0.5,  1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5,  0.5,  0.5,  1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5,  0.5, -0.5,  1.0,    -1.0, 0.0, 0.0, 1.0,
    -0.5, -0.5, -0.5,  1.0,    -1.0, 0.0, 0.0, 1.0,
    // bottom
    -0.5, -0.5, -0.5,  1.0,    0.0, -1.0, 0.0, 1.0,
    0.5, -0.5, -0.5,  1.0,    0.0, -1.0, 0.0, 1.0,
    0.5, -0.5,  0.5,  1.0,    0.0, -1.0, 0.0, 1.0,
    0.5, -0.5,  0.5,  1.0,    0.0, -1.0, 0.0, 1.0,
    -0.5, -0.5,  0.5,  1.0,    0.0, -1.0, 0.0, 1.0,
    -0.5, -0.5, -0.5,  1.0,    0.0, -1.0, 0.0, 1.0,
    // top
    -0.5,  0.5,  0.5,  1.0,    0.0, 1.0, 0.0, 1.0,
    0.5,  0.5,  0.5,  1.0,    0.0, 1.0, 0.0, 1.0,
    0.5,  0.5, -0.5,  1.0,    0.0, 1.0, 0.0, 1.0,
    0.5,  0.5, -0.5,  1.0,    0.0, 1.0, 0.0, 1.0,
    -0.5,  0.5, -0.5,  1.0,    0.0, 1.0, 0.0, 1.0,
    -0.5,  0.5,  0.5,  1.0,    0.0, 1.0, 0.0, 1.0,
};

// Vertex layout descriptor, 2 streams (0 and 1) with 4 floats each
VertexFormatDesc desc[] = {
    0, FLOAT, 4,
    1, FLOAT, 4
};

bool quit = false;

int main(int argc, const char * argv[])
{
    // Initialize everything
    AppContext appContext;
    appContext.InitApp("OctoGL v0.0.0");
    
    Renderer renderer(BACKEND_OPENGL, 512);
    
    Camera camera;
    camera.setViewSize(glm::vec2(appContext.GetWindow()->GetWidth(),
                                 appContext.GetWindow()->GetHeight()));
    camera.setPosition(glm::vec3(1.5, 1.5, 1.5));
    camera.setRotation(glm::vec3(-M_PI_4, M_PI_4, 0.0));
    
    VertexBufferID vbID = renderer.backend().addVertexBuffer(sizeof(Cube_3D),
                                                             STATIC,
                                                             Cube_3D);
    
    VertexBufferID vbids[2] = { vbID, vbID };
    VertexLayoutID vlID = renderer.backend().addVertexLayout(2,
                                                             desc,
                                                             vbids);
    
    SDL_Event event;
    
    // Main application loop
    while (!quit) {
        camera.Update(0.01666f);
        
        commands::Draw* dc = renderer.drawStack().AddCommand<commands::Draw>(1, 0);
        dc->vertexBufferHandle = vbID;
        dc->vertexLayoutHandle = vlID;
        dc->vertexCount = 36;
        dc->primitiveMode = DRAW_TRIANGLES;
        
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
        
        renderer.Flush(camera);
        // Swap buffers, this actually puts the stuff we drew before on the screen
        appContext.GetWindow()->SwapBuffers();
    }
    
    return 0;
}
