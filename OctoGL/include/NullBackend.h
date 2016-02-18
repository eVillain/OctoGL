#ifndef NULL_BACKEND_H
#define NULL_BACKEND_H

#include "RenderBackend.h"

class NullBackend
{
public:
    NullBackend();
    virtual ~NullBackend();
    
    virtual VertexBufferID addVertexBuffer(const long size,
                                           const BufferAccess bufferAccess,
                                           const void* data = NULL);
    virtual void setVertexBuffer(const VertexBufferID vb);
    virtual void uploadVertexData(const long size,
                                  const BufferAccess bufferAccess,
                                  const void* data);
    
    virtual VertexLayoutID addVertexLayout(const long numStreams,
                                           const VertexFormatDesc* formats,
                                           const VertexBufferID* vbIDs);
    virtual void setVertexLayout(const VertexLayoutID vl);
};

#endif /* NULL_BACKEND_H */
