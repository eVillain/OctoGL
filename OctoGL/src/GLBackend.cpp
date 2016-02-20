#include "GLBackend.h"
#include <GL/glew.h>
#include <cstdlib>

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

struct Shader {
    GLuint programHandle;
    GLuint fragmentHandle;
    GLuint vertexHandle;
    GLuint geometryHandle;
};

struct DepthState {
    GLenum depthFunc;
    bool depthTest;
    bool depthWrite;
};

const GLenum VBAccess[] = {
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW,
    GL_STREAM_DRAW,
};

const GLenum GLVertexDataTypes[] = {
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_FLOAT,
    GL_DOUBLE,
};

const GLenum GLDrawPrimitives[] = {
    GL_TRIANGLES,
    GL_TRIANGLE_FAN,
    GL_TRIANGLE_STRIP,
    GL_QUADS,
    GL_LINES,
    GL_LINE_STRIP,
    GL_LINE_LOOP,
    GL_POINTS,
};

const GLenum GLDepthTests[] = {
    GL_NEVER,
    GL_LESS,
    GL_EQUAL,
    GL_LEQUAL,
    GL_GREATER,
    GL_NOTEQUAL,
    GL_GEQUAL,
    GL_ALWAYS,
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
    printf("[GLBackend] ERROR GETTING DATA SIZE - UNKNOWN ENUM TYPE!\n");
    return 0;
}

GLBackend::GLBackend() :
_currentVertexBuffer(255),
_currentVertexLayout(255),
_currentShader(255),
_currentDepthState(255)
{
    
}

GLBackend::~GLBackend()
{
    for (auto vb : _vertexBuffers.getMap()) {
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

    VertexBufferID newID = _vertexBuffers.add(vb);
    _currentVertexBuffer = newID;
    return newID;
}

void GLBackend::setVertexBuffer(const VertexBufferID vb)
{
    if (vb != _currentVertexBuffer) {
        glBindBuffer(GL_ARRAY_BUFFER,
                     _vertexBuffers.getMap().at(vb).vboHandle);
        _currentVertexBuffer = vb;
    }
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
    
    VertexLayoutID newID = _vertexLayouts.add(vl);
    _currentVertexLayout = newID;
    return newID;
}

void GLBackend::setVertexLayout(const VertexLayoutID vl)
{
    if (vl != _currentVertexLayout) {
        glBindVertexArray(_vertexLayouts.getMap().at(vl).vaoHandle);
        _currentVertexLayout = vl;
    }
}

ShaderID GLBackend::addShader(const char* fragSource,
                              const char* vertSource,
                              const char* geomSource)
{
    Shader s;
    
    s.fragmentHandle = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (s.fragmentHandle, 1, &fragSource, NULL);
    glCompileShader (s.fragmentHandle);
    
    s.vertexHandle = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (s.vertexHandle, 1, &vertSource, NULL);
    glCompileShader (s.vertexHandle);

    if (geomSource != NULL) {
        s.geometryHandle = glCreateShader (GL_GEOMETRY_SHADER);
        glShaderSource (s.geometryHandle, 1, &geomSource, NULL);
        glCompileShader (s.geometryHandle);
    }
    
    s.programHandle = glCreateProgram();
    glAttachShader(s.programHandle, s.fragmentHandle);
    glAttachShader(s.programHandle, s.vertexHandle);
    
    if (geomSource != NULL) {
        glAttachShader(s.programHandle, s.geometryHandle);
    }
    glLinkProgram(s.programHandle);
    
    // Check for errors
    GLint result;
    glGetProgramiv(s.programHandle, GL_LINK_STATUS, &result);
    if(result == GL_FALSE) {
        GLint length;
        char *log;
        glGetProgramiv(s.programHandle, GL_INFO_LOG_LENGTH, &length);
        log = (char*)malloc(length);
        glGetProgramInfoLog(s.programHandle, length, &result, log);
        fprintf(stderr, "Shader program linking failed: %s\n", log);
        free(log);
        s.programHandle = 0;
    }
    
    ShaderID newID = _shaders.add(s);
    return newID;
}

void GLBackend::setShader(const ShaderID s)
{
    if (s != _currentShader) {
        glUseProgram(_shaders.getMap().at(s).programHandle);
        _currentShader = s;
    }
}

void GLBackend::setShaderConstant1i(const char *name,
                                    const int constant)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniform1i(constantID, constant);
}

void GLBackend::setShaderConstant1f(const char *name,
                                    const float constant)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniform1f(constantID, constant);
}


void GLBackend::setShaderConstant2f(const char *name,
                                    const float* constant)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniform2fv(constantID, 1, constant);
}


void GLBackend::setShaderConstant3f(const char *name,
                                    const float* constant)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniform3fv(constantID, 1, constant);
}


void GLBackend::setShaderConstant4f(const char *name,
                                    const float* constant)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniform4fv(constantID, 1, constant);
}


void GLBackend::setShaderConstant4x4f(const char *name,
                                      const float* constant)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniformMatrix4fv(constantID, 1, GL_FALSE, constant);
}


void GLBackend::setShaderConstantArray1f(const char *name,
                                         const float *constant,
                                         const unsigned int count)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniform1fv(constantID, count, constant);

}


void GLBackend::setShaderConstantArray2f(const char *name,
                                         const float* constant,
                                         const unsigned int count)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniform2fv(constantID, count, constant);
}


void GLBackend::setShaderConstantArray3f(const char *name,
                                         const float* constant,
                                         const unsigned int count)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniform3fv(constantID, count, constant);
}


void GLBackend::setShaderConstantArray4f(const char *name,
                                         const float* constant,
                                         const unsigned int count)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniform4fv(constantID, count, constant);
}


void GLBackend::setShaderConstantArray4x4f(const char *name,
                                           const float* constant,
                                           const unsigned int count)
{
    GLuint constantID = glGetUniformLocation(_shaders.getMap().at(_currentShader).programHandle,
                                             name);
    glUniformMatrix4fv(constantID, count, GL_FALSE, constant);
}

DepthStateID GLBackend::addDepthState(const bool depthTest,
                                      const bool depthWrite,
                                      const DepthTestType depthFunc)
{
    DepthState ds;
    
    ds.depthTest = depthTest;
    ds.depthWrite = depthWrite;
    ds.depthFunc = GLDepthTests[depthFunc];
    
    DepthStateID newID = _depthStates.add(ds);
    return newID;
}

void GLBackend::setDepthState(const DepthStateID ds)
{
    DepthState state = _depthStates.getMap().at(ds);
    if (ds != _currentDepthState)
    {
        state.depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        state.depthWrite ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
        glDepthFunc(state.depthFunc);
    }
}

void GLBackend::clear(const bool clearColor,
                      const bool clearDepth,
                      const bool clearStencil,
                      const float *color,
                      const float depth,
                      const uint8_t stencil)
{
    GLbitfield clearBits = 0;
    
    if (clearColor) {
        clearBits |= GL_COLOR_BUFFER_BIT;
        glClearColor(color[0], color[1], color[2], color[3]);
        
    }
    if (clearDepth) {
        clearBits |= GL_DEPTH_BUFFER_BIT;
        glClearDepth(depth);
    }
    if (clearStencil) {
        clearBits |= GL_STENCIL_BUFFER_BIT;
        glClearStencil(stencil);
    }
    
    if (clearBits) {
        glClear(clearBits);
    }
}

void GLBackend::drawArrays(const DrawPrimitives primitives,
                           const int firstVertex,
                           const int vertexCount)
{
    glDrawArrays(GLDrawPrimitives[primitives],
                 firstVertex,
                 vertexCount);
    
    _drawCallCount++;
}
