#ifndef COMMANDS_H
#define COMMANDS_H

#include "RendererBackend.h"
#include "BackendDispatch.h"
#include <type_traits>

namespace commands
{
    struct Draw
    {
        static const BackendDispatchFunction DISPATCH_FUNCTION;
        
        uint32_t vertexCount;
        uint32_t startVertex;
        DrawPrimitives primitiveMode;
        
        VertexBufferID vertexBufferHandle;
        VertexLayoutID vertexLayoutHandle;
    };
    static_assert(std::is_pod<Draw>::value == true, "Draw must be a POD.");
    
    // TODO: implement DrawInstanced command
    
    struct BufferVertexData
    {
        static const BackendDispatchFunction DISPATCH_FUNCTION;
        
        VertexBufferID vertexBufferHandle;
        VertexLayoutID vertexLayoutHandle;
        
        uint32_t size;
        void* data;
    };
    static_assert(std::is_pod<BufferVertexData>::value == true, "BufferVertexData must be a POD.");

    
}


#endif /* COMMANDS_H */
