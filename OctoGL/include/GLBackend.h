#ifndef GL_BACKEND_H
#define GL_BACKEND_H

#include "RendererBackend.h"
#include "IndexedMap.h"

class GLBackend : public RendererBackend
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

    // -- Shaders -- //
    virtual ShaderID addShader(const char* fragSource,
                               const char* vertSource,
                               const char* geomSource = NULL);
    virtual void setShader(const ShaderID shaderID);

    virtual void setShaderConstantMat4(const char* name,
                                       const float* data);
    
    // -- Render functionality -- //
    virtual void clear(const bool clearColor,
                       const bool clearDepth,
                       const bool clearStencil,
                       const float *color,
                       const float depth,
                       const uint8_t stencil);
    
    virtual void drawArrays(const DrawPrimitives primitives,
                            const int firstVertex,
                            const int nVertices);
    
private:
    IndexedMap<VertexBufferID, VertexBuffer> _vertexBuffers;
    IndexedMap<VertexLayoutID, VertexLayout> _vertexLayouts;
    IndexedMap<ShaderID, Shader> _shaders;
    
    ShaderID _currentShader;
};
#endif /* GLBackend_hpp */