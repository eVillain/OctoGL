#include "GLBackend.h"
// This is only for AppleGL, in the real world we need to wrap this with some
// preprocessor directives to support other platforms
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
    for (auto vb : vertexBuffers.getMap()) {
        glDeleteBuffers(1, &vb.second.vboHandle);
    }
}

VertexBufferID GLBackend::addVertexBuffer(const long size,
                                          const BufferAccess bufferAccess,
                                          const void *data)
{
    VertexBuffer vb;
    vb.size = (GLsizei)size;

    glGenBuffers(1, &vb.vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER,
                 vb.vboHandle);
    glBufferData(GL_ARRAY_BUFFER,
                 size,
                 data,
                 VBAccess[bufferAccess]);

    VertexBufferID newID = vertexBuffers.add(vb);
    return newID;
}

void GLBackend::setVertexBuffer(const VertexBufferID vb)
{
    glBindBuffer(GL_ARRAY_BUFFER,
                 vertexBuffers.getMap().at(vb).vboHandle);
}

void GLBackend::uploadVertexData(const long size,
                                 const BufferAccess bufferAccess,
                                 const void* data)
{
    glBufferData(GL_ARRAY_BUFFER,
                 size,
                 data,
                 VBAccess[bufferAccess]);
}
