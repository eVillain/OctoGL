#include "RendererBackend.h"

RendererBackend::RendererBackend()
{
    // Base init will go here
}

RendererBackend::~RendererBackend()
{
    // Base cleanup here
}

void RendererBackend::resetDrawCallCounter()
{
    _drawCallCount = 0;
}
