#include "BackendDispatch.h"
#include "Commands.h"
#include "NullBackend.h"

RendererBackend* _backend = NULL;

void BackendDispatch::setBackend(RendererBackend* backend)
{
    if (backend != _backend) {
        _backend = backend;
    }
}

void BackendDispatch::draw(const void* data)
{
    const commands::Draw* realData = reinterpret_cast<const commands::Draw*>(data);
    _backend->drawArrays(realData->primitiveMode, realData->startVertex, realData->vertexCount);
}

void BackendDispatch::drawInstanced(const void* data)
{
//            const commands::DrawInstanced* realData = reinterpret_cast<const commands::DrawInstanced*>(data);
//            Backend::DrawInstanced(realData->mode, realData->vertexCount, realData->startVertex);
}

void BackendDispatch::bufferVertexData(const void* data)
{
    const commands::BufferVertexData* realData = reinterpret_cast<const commands::BufferVertexData*>(data);
    _backend->uploadVertexData(realData->size, STATIC, realData->data);
}


