#ifndef COMMANDPACKET_H
#define COMMANDPACKET_H

#include "BackendDispatch.h"
#include <stddef.h>

typedef void* CommandPacket;

namespace commandPacket
{
    static const size_t OFFSET_NEXT_COMMAND_PACKET = 0u;
    static const size_t OFFSET_BACKEND_DISPATCH_FUNCTION = OFFSET_NEXT_COMMAND_PACKET + sizeof(CommandPacket);
    static const size_t OFFSET_COMMAND = OFFSET_BACKEND_DISPATCH_FUNCTION + sizeof(BackendDispatchFunction);
    
    template <typename T>
    static size_t GetSize(size_t auxMemorySize)
    {
        return OFFSET_COMMAND + sizeof(T) + auxMemorySize;
    };
    
    template <typename T>
    static CommandPacket Create(size_t auxMemorySize)
    {
        return ::operator new(GetSize<T>(auxMemorySize));
    }
    
    static CommandPacket* GetNextCommandPacket(CommandPacket packet)
    {
        return reinterpret_cast<CommandPacket*>(reinterpret_cast<char*>(packet) + OFFSET_NEXT_COMMAND_PACKET);
    }
    
    template <typename T>
    static CommandPacket* GetNextCommandPacket(T* command)
    {
        return reinterpret_cast<CommandPacket*>(reinterpret_cast<char*>(command) - OFFSET_COMMAND + OFFSET_NEXT_COMMAND_PACKET);
    }
    
    static BackendDispatchFunction* GetBackendDispatchFunction(CommandPacket packet)
    {
        return reinterpret_cast<BackendDispatchFunction*>(reinterpret_cast<char*>(packet) + OFFSET_BACKEND_DISPATCH_FUNCTION);
    }
    
    template <typename T>
    static T* GetCommand(CommandPacket packet)
    {
        return reinterpret_cast<T*>(reinterpret_cast<char*>(packet) + OFFSET_COMMAND);
    }
    
    template <typename T>
    static char* GetAuxiliaryMemory(T* command)
    {
        return reinterpret_cast<char*>(command) + sizeof(T);
    }
    
    static inline void StoreNextCommandPacket(CommandPacket packet, CommandPacket nextPacket)
    {
        *commandPacket::GetNextCommandPacket(packet) = nextPacket;
    }
    
    template <typename T>
    static void StoreNextCommandPacket(T* command, CommandPacket nextPacket)
    {
        *commandPacket::GetNextCommandPacket<T>(command) = nextPacket;
    }
    
    static inline void StoreBackendDispatchFunction(CommandPacket packet, BackendDispatchFunction dispatchFunction)
    {
        *commandPacket::GetBackendDispatchFunction(packet) = dispatchFunction;
    }
    
    static inline const CommandPacket LoadNextCommandPacket(const CommandPacket packet)
    {
        return *GetNextCommandPacket(packet);
    }
    
    static inline const BackendDispatchFunction LoadBackendDispatchFunction(const  CommandPacket packet)
    {
        return *GetBackendDispatchFunction(packet);
    }
    
    static inline const void* LoadCommand(const CommandPacket packet)
    {
        return reinterpret_cast<char*>(packet) + OFFSET_COMMAND;
    }
}

#endif /* COMMANDPACKET_H */
