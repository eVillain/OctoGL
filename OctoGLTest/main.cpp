#include "AppContext.h"
#include "GLBackend.h"

#include "DefaultShaders.h"
#include <SDL2/SDL.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
const VertexFormatDesc desc[] = {
    0, FLOAT, 4,
    1, FLOAT, 4
};

glm::mat4 getMVP(int width, int height)
{
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
    
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    
    // Camera matrix
    glm::mat4 View = glm::lookAt(glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                                 glm::vec3(0,0,0), // and looks at the origin
                                 glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                 );
    
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
    return mvp;
}

bool quit = false;

int main(int argc, const char * argv[])
{
    // Initialize everything
    AppContext appContext;
    appContext.InitApp("OctoGL v0.0.0");
    
    GLBackend backend;
    
    VertexBufferID vbID = backend.addVertexBuffer(sizeof(Cube_3D),
                                                  STATIC,
                                                  Cube_3D);
    
    VertexLayoutID vlID = backend.addVertexLayout(2,
                                                  desc,
                                                  &vbID);
    
    ShaderID shaderID = backend.addShader(fragment_shader_color,
                                          vertex_shader_normalvis);
    
    DepthStateID depthState = backend.addDepthState(true, true, LESS);
    SDL_Event event;
    
    glm::mat4 mvp = getMVP(appContext.GetWindow()->GetWidth(),
                           appContext.GetWindow()->GetHeight());
    
    // Main application loop
    while (!quit) {
        // First wipe the drawing surface clear
        float clearColor[4] = { 0.3, 0.3, 0.3, 0.0 };
        backend.clear(true, true, false, clearColor, 1.0, 0);
        
        // Set our render state
        backend.setDepthState(depthState);
        backend.setShader(shaderID);
        backend.setShaderConstant4x4f("MVP", glm::value_ptr(mvp));
        backend.setVertexLayout(vlID);
        
        // Draw points 0-36 from the currently bound vertices with current shader
        backend.drawArrays(DRAW_TRIANGLES, 0, 36);
        
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
