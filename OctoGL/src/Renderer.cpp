#include "Renderer.h"
#include "NullBackend.h"
#include "GLBackend.h"
#include "DefaultShaders.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float COLOR_CLEAR_BLACK[4] = { 0.0, 0.0, 0.0, 0.0 };

Renderer::Renderer(const BackendType backend,
                   const uint32_t stackSize) :
_backend(nullptr),
_drawStack(std::make_unique<CommandStack<uint64_t>>(stackSize))
{
    if (backend == BACKEND_OPENGL) {
        _backend = std::make_unique<GLBackend>();
    } else {
        _backend = std::make_unique<NullBackend>();
    }
    
    _defaultDepthState = _backend->addDepthState(true, true, LEQUAL);
    _defaultShaderID = _backend->addShader(fragment_shader_color,
                                          vertex_shader_normalvis);

    BackendDispatch::setBackend(_backend.get());
}

Renderer::~Renderer()
{ }

void Renderer::Flush(const Camera& camera)
{
    // First wipe the drawing surface clear
    _backend->clear(true, true, false, COLOR_CLEAR_BLACK, 1.0, 0);
    
    // Set our render state
    _backend->setDepthState(_defaultDepthState);

    glm::mat4 mvp = camera.getMVP();
    _backend->setShader(_defaultShaderID);
    _backend->setShaderConstant4x4f("MVP", glm::value_ptr(mvp));

    // The stacks will need a sorting method before they can really do their job
//    _drawStack->Sort();
    _drawStack->Submit();
}