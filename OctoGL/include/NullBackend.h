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

};

#endif /* NULL_BACKEND_H */
