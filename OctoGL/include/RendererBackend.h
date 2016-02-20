#ifndef RENDERER_BACKEND_H
#define RENDERER_BACKEND_H

#include <cstdint>

typedef uint8_t VertexBufferID;
typedef uint8_t VertexLayoutID;
typedef uint8_t ShaderID;
typedef uint8_t DepthStateID;

struct VertexBuffer;
struct VertexLayout;
struct Shader;
struct DepthState;

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

enum DrawPrimitives {
    DRAW_TRIANGLES      = 0,
    DRAW_TRIANGLE_FAN   = 1,
    DRAW_TRIANGLE_STRIP = 2,
    DRAW_QUADS          = 3,
    DRAW_LINES          = 4,
    DRAW_LINE_STRIP     = 5,
    DRAW_LINE_LOOP      = 6,
    DRAW_POINTS         = 7,
};

enum DepthTestType {
    NEVER,
    LESS,
    EQUAL,
    LEQUAL,
    GREATER,
    NOTEQUAL,
    GEQUAL,
    ALWAYS,
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
    
    // -- Vertex buffers -- //
    virtual VertexBufferID addVertexBuffer(const long size,
                                           const BufferAccess bufferAccess,
                                           const void* data = NULL) = 0;
    virtual void setVertexBuffer(const VertexBufferID vbID) = 0;
    virtual void uploadVertexData(const long size,
                                  const BufferAccess bufferAccess,
                                  const void* data) = 0;
    
    // -- Vertex layouts -- //
    virtual VertexLayoutID addVertexLayout(const long numStreams,
                                           const VertexFormatDesc* formats,
                                           const VertexBufferID* vbIDs) = 0;
    virtual void setVertexLayout(const VertexLayoutID vlID) = 0;
    
    // -- Shaders -- //
    virtual ShaderID addShader(const char* fragSource,
                               const char* vertSource,
                               const char* geomSource = NULL) = 0;
    virtual void setShader(const ShaderID shaderID) = 0;
    virtual void setShaderConstant1i(const char *name,
                                     const int constant) = 0;
    virtual void setShaderConstant1f(const char *name,
                                     const float constant) = 0;
    virtual void setShaderConstant2f(const char *name,
                                     const float* constant) = 0;
    virtual void setShaderConstant3f(const char *name,
                                     const float* constant) = 0;
    virtual void setShaderConstant4f(const char *name,
                                     const float* constant) = 0;
    virtual void setShaderConstant4x4f(const char *name,
                                       const float* constant) = 0;
    virtual void setShaderConstantArray1f(const char *name,
                                          const float *constant,
                                          const unsigned int count) = 0;
    virtual void setShaderConstantArray2f(const char *name,
                                          const float* constant,
                                          const unsigned int count) = 0;
    virtual void setShaderConstantArray3f(const char *name,
                                          const float* constant,
                                          const unsigned int count) = 0;
    virtual void setShaderConstantArray4f(const char *name,
                                          const float* constant,
                                          const unsigned int count) = 0;
    virtual void setShaderConstantArray4x4f(const char *name,
                                            const float* constant,
                                            const unsigned int count) = 0;
    // -- Depth test -- //
    virtual DepthStateID addDepthState(const bool depthTest,
                                       const bool depthWrite,
                                       const DepthTestType depthFunc) = 0;
    virtual void setDepthState(const DepthStateID ds) = 0;

    // -- Render functionality -- //
    virtual void clear(const bool clearColor,
                       const bool clearDepth,
                       const bool clearStencil,
                       const float *color,
                       const float depth,
                       const uint8_t stencil) = 0;
    
    virtual void drawArrays(const DrawPrimitives primitives,
                            const int firstVertex,
                            const int vertexCount) = 0;
    
    // -- Statistics -- //
    uint64_t getDrawCallCount() { return _drawCallCount; }
    void resetDrawCallCounter();
    
protected:
    uint64_t _drawCallCount;
};

#endif /* RENDERER_BACKEND_H */
