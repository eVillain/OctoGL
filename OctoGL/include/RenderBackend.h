#ifndef RENDER_BACKEND_H
#define RENDER_BACKEND_H

#include <cstdint>

typedef uint8_t VertexBufferID;

struct VertexBuffer;

enum BufferAccess {
    STATIC,
    DEFAULT,
    DYNAMIC,
};

class RendererBackend
{
public:
    RendererBackend();
    virtual ~RendererBackend();
    
    virtual VertexBufferID addVertexBuffer(const long size,
                                           const BufferAccess bufferAccess,
                                           const void* data = NULL) = 0;
    virtual void setVertexBuffer(const VertexBufferID vb) = 0;
    virtual void uploadVertexData(const long size,
                                  const BufferAccess bufferAccess,
                                  const void* data) = 0;

};

#endif /* RENDER_BACKEND_H */
