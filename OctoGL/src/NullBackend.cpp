#include "NullBackend.h"

NullBackend::NullBackend()
{ }

NullBackend::~NullBackend()
{ }

VertexBufferID NullBackend::addVertexBuffer(const long size,
                                            const BufferAccess bufferAccess,
                                            const void *data)
{
    return 0;
}

void NullBackend::setVertexBuffer(const VertexBufferID vb)
{ }

void NullBackend::uploadVertexData(const long size,
                                   const BufferAccess bufferAccess,
                                   const void* data)
{ }

VertexLayoutID NullBackend::addVertexLayout(const long numStreams,
                                       const VertexFormatDesc* formats,
                                       const VertexBufferID* vbIDs)
{
    return 0;
}

void NullBackend::setVertexLayout(const VertexLayoutID vl)
{ }

ShaderID NullBackend::addShader(const char* fragSource,
                                const char* vertSource,
                                const char* geomSource)
{
    return 0;
}

void NullBackend::setShader(const ShaderID shaderID)
{ }

void NullBackend::setShaderConstant1i(const char *name,
                                      const int constant)
{ }

void NullBackend::setShaderConstant1f(const char *name,
                                      const float constant)
{ }

void NullBackend::setShaderConstant2f(const char *name,
                                      const float* constant)
{ }

void NullBackend::setShaderConstant3f(const char *name,
                                      const float* constant)
{ }

void NullBackend::setShaderConstant4f(const char *name,
                                      const float* constant)
{ }

void NullBackend::setShaderConstant4x4f(const char *name,
                                        const float* constant)
{ }

void NullBackend::setShaderConstantArray1f(const char *name,
                                           const float *constant,
                                           const unsigned int count)
{ }

void NullBackend::setShaderConstantArray2f(const char *name,
                                           const float* constant,
                                           const unsigned int count)
{ }

void NullBackend::setShaderConstantArray3f(const char *name,
                                           const float* constant,
                                           const unsigned int count)
{ }

void NullBackend::setShaderConstantArray4f(const char *name,
                                           const float* constant,
                                           const unsigned int count)
{ }

void NullBackend::setShaderConstantArray4x4f(const char *name,
                                             const float* constant,
                                             const unsigned int count)
{ }

DepthStateID NullBackend::addDepthState(const bool depthTest,
                                        const bool depthWrite,
                                        const DepthTestType depthFunc)
{
    return 0;
}
