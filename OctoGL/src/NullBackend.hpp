#ifndef NullBackend_hpp
#define NullBackend_hpp

#include "RenderBackend.hpp"

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

#endif /* NullBackend_hpp */
