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