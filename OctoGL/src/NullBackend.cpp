#include "NullBackend.hpp"

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
