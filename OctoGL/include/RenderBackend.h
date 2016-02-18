#ifndef RENDER_BACKEND_H
#define RENDER_BACKEND_H

#include <cstdint>

typedef uint8_t VertexBufferID;
typedef uint8_t VertexLayoutID;

struct VertexBuffer;
struct VertexLayout;

enum BufferAccess {
    STATIC,
    DEFAULT,
    DYNAMIC,
};

enum VertexDataType {
    BYTE,
    UNSIGNED_BYTE,
    SHORT,
    UNSIGNED_SHORT,
    INT,
    UNSIGNED_INT,
    FLOAT,
    DOUBLE,
};

struct VertexFormatDesc {
    int stream;
    VertexDataType type;
    int size;
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

    virtual VertexLayoutID addVertexLayout(const long numStreams,
                                           const VertexFormatDesc* formats,
                                           const VertexBufferID* vbIDs) = 0;
    virtual void setVertexLayout(const VertexLayoutID vl) = 0;

};

#endif /* RENDER_BACKEND_H */
