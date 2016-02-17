#include "GLBackend.hpp"
#include <AGL/AGL.h>

struct VertexBuffer {
    GLuint vboHandle;
    GLsizei size;
};

GLenum VBAccess[] = {
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW,
    GL_STREAM_DRAW,
};

GLBackend::GLBackend()
{
    
}

GLBackend::~GLBackend()
{
    for (auto vb : vertexBuffers) {
        glDeleteBuffers(1, &vb.vboHandle);
    }
    vertexBuffers.clear();
}

VertexBufferID GLBackend::addVertexBuffer(const long size,
                                          const BufferAccess bufferAccess,
                                          const void *data)
{
    VertexBuffer vb;
    vb.size = (GLsizei)size;

    glGenBuffers(1, &vb.vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vb.vboHandle);
    glBufferData(GL_ARRAY_BUFFER, size, data, VBAccess[bufferAccess]);

    vertexBuffers.push_back(vb);
    
    // This is obviously not a proper way to index things :)
    return vertexBuffers.size()-1;
}

void GLBackend::setVertexBuffer(const VertexBufferID vb)
{
    glBindBuffer (GL_ARRAY_BUFFER, vertexBuffers[vb].vboHandle);
}

void GLBackend::uploadVertexData(const long size,
                                 const BufferAccess bufferAccess,
                                 const void* data)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, VBAccess[bufferAccess]);
}
