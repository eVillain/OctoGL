#ifndef RENDERER_H
#define RENDERER_H

#include "CommandStack.h"
#include <memory>

enum BackendType {
    BACKEND_NULL,
    BACKEND_OPENGL,
};

class RendererBackend;
class Camera;

class Renderer
{
public:
    Renderer(const BackendType backend,
             const uint32_t stackSize);
    ~Renderer();
    
    void Flush(const Camera& camera);
    
    RendererBackend& backend() { return *_backend.get(); }
    CommandStack<uint64_t>& drawStack() { return *_drawStack.get(); }
private:
    std::unique_ptr<RendererBackend> _backend;
    // This should eventually grow to hold multiple stacks, one for each render pass:
    // G-Buffer, Lighting, Deferred, PostProcessing, GUI
    std::unique_ptr<CommandStack<uint64_t>> _drawStack;
    
    DepthStateID _defaultDepthState;
    ShaderID _defaultShaderID;
    
};

#endif /* RENDERER_H */
