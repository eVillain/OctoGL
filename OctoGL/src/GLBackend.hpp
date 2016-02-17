#ifndef GLBackend_hpp
#define GLBackend_hpp

#include "RenderBackend.hpp"
#include <vector>

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
    std::vector<VertexBuffer> vertexBuffers;

};
#endif /* GLBackend_hpp */
