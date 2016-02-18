#include "GLBackend.h"
#include <GL/glew.h>

// This is only for AppleGL, in the real world we need to wrap this with some
// preprocessor directives to support other platforms
//#include <AGL/AGL.h>

struct VertexBuffer {
    GLuint vboHandle;
    GLsizei size;
};

struct VertexLayout {
    GLuint vaoHandle;
};

GLenum VBAccess[] = {
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW,
    GL_STREAM_DRAW,
};

GLenum GLVertexDataTypes[] = {
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_FLOAT,
    GL_DOUBLE,
};


GLsizei getDataSize(GLenum type){
    switch (type){
        case GL_BYTE:           return sizeof(GLfloat);
        case GL_UNSIGNED_BYTE:  return sizeof(GLuint);
        case GL_SHORT:          return sizeof(GLshort);
        case GL_UNSIGNED_SHORT: return sizeof(GLushort);
        case GL_INT:            return sizeof(GLint);
        case GL_UNSIGNED_INT:   return sizeof(GLuint);
        case GL_FLOAT:          return sizeof(GLfloat);
        case GL_DOUBLE:         return sizeof(GLdouble);
    }
    printf("ERROR GETTING DATA SIZE - UNKNOWN ENUM TYPE!\n");
    return 0;
}

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

VertexLayoutID GLBackend::addVertexLayout(const long numStreams,
                                          const VertexFormatDesc* formats,
                                          const VertexBufferID* vbIDs)
{
    VertexLayout vl;
    glGenVertexArrays(1, &vl.vaoHandle);
    glBindVertexArray(vl.vaoHandle);
    
    GLsizei stride = 0;
    unsigned long offset = 0;
    for (GLuint stream=0; stream < numStreams; stream++) {
        VertexFormatDesc desc = formats[stream];
        stride += desc.size * getDataSize(GLVertexDataTypes[desc.type]);
    }

    for (GLuint stream=0; stream < numStreams; stream++) {
        glEnableVertexAttribArray(stream);
        setVertexBuffer(vbIDs[stream]);
        
        VertexFormatDesc desc = formats[stream];
        glVertexAttribPointer(stream,
                              desc.size,
                              GLVertexDataTypes[desc.type],
                              GL_FALSE,
                              stride,
                              (GLvoid*)offset);
        offset += getDataSize(GLVertexDataTypes[desc.type]);
    }
    
    VertexLayoutID newID = vertexLayouts.add(vl);
    return newID;
}

void GLBackend::setVertexLayout(const VertexLayoutID vl)
{
    glBindVertexArray(vertexLayouts.getMap().at(vl).vaoHandle);
}
