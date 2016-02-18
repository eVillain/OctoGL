#ifndef GL_BACKEND_H
#define GL_BACKEND_H

#include "RenderBackend.h"
#include "IndexedMap.h"

class GLBackend
{
public:
    GLBackend();
    virtual ~GLBackend();
    
    virtual VertexBufferID addVertexBuffer(const long size,
                                           const BufferAccess bufferAccess,
                                           const void* data = NULL);
    virtual void setVertexBuffer(const VertexBufferID vb);
    virtual void uploadVertexData(const long size,
                                  const BufferAccess bufferAccess,
                                  const void* data);
    
private:
    IndexedMap<VertexBufferID, VertexBuffer> vertexBuffers;

};
#endif /* GLBackend_hpp */
