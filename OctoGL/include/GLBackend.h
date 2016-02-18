#ifndef GL_BACKEND_H
#define GL_BACKEND_H

#include "RenderBackend.h"
#include "IndexedMap.h"

class GLBackend
{
public:
    GLBackend();
    virtual ~GLBackend();
    
    // -- Vertex buffers -- //
    virtual VertexBufferID addVertexBuffer(const long size,
                                           const BufferAccess bufferAccess,
                                           const void* data = NULL);
    virtual void setVertexBuffer(const VertexBufferID vb);
    virtual void uploadVertexData(const long size,
                                  const BufferAccess bufferAccess,
                                  const void* data);
    
    // -- Vertex layouts -- //
    virtual VertexLayoutID addVertexLayout(const long numStreams,
                                           const VertexFormatDesc* formats,
                                           const VertexBufferID* vbIDs);
    virtual void setVertexLayout(const VertexLayoutID vl);

private:
    IndexedMap<VertexBufferID, VertexBuffer> vertexBuffers;
    IndexedMap<VertexLayoutID, VertexLayout> vertexLayouts;

};
#endif /* GLBackend_hpp */
