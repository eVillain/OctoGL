#include "Commands.h"

namespace commands
{
    const BackendDispatchFunction Draw::DISPATCH_FUNCTION = &BackendDispatch::draw;
    const BackendDispatchFunction BufferVertexData::DISPATCH_FUNCTION = &BackendDispatch::bufferVertexData;
}
