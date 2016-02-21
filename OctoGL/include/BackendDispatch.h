#ifndef BACKEND_DISPATCH_H
#define BACKEND_DISPATCH_H

#include "RendererBackend.h"

typedef void (*BackendDispatchFunction)(const void*);

namespace BackendDispatch
{
    void setBackend(RendererBackend* backend);
    
    void draw(const void* data);
    
    void drawInstanced(const void* data);
    
    void bufferVertexData(const void* data);
};


#endif /* BACKEND_DISPATCH_H */
