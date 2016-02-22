#ifndef COMMANDSTACK_H
#define COMMANDSTACK_H

#include "CommandPacket.h"
#include <cstdlib>

template <typename T>
class CommandStack
{
public:
    typedef T Key;
    
    CommandStack(const uint32_t size)
    {
        _keys = new Key[size];
        _data = (void**)malloc(size*sizeof(void*));
        
        _packets = new CommandPacket[size];
        _current = 0;
    }
    
    
    ~CommandStack()
    {
        delete [] _keys;
        delete [] _data;
        delete [] _packets;
    }
    
    
    template <typename U>
    U* AddCommand(Key key, size_t auxMemorySize)
    {
        CommandPacket packet = commandPacket::Create<U>(auxMemorySize);
        
        // store key and pointer to the data
        {
            // TODO: add some kind of lock or atomic operation here
            const unsigned int current = _current++;
            _keys[current] = key;
            _packets[current] = packet;
        }
        
        commandPacket::StoreNextCommandPacket(packet, nullptr);
        commandPacket::StoreBackendDispatchFunction(packet, U::DISPATCH_FUNCTION);
        
        return commandPacket::GetCommand<U>(packet);
    }
    
    void Submit(void)
    {
        
        for (unsigned int i=0; i < _current; ++i)
        {
            CommandPacket packet = _packets[i];
            do
            {
                SubmitPacket(packet);
                packet = commandPacket::LoadNextCommandPacket(packet);
            } while (packet != nullptr);
        }
        _current = 0;
    }
    
private:
    void SubmitPacket(const CommandPacket packet)
    {
        const BackendDispatchFunction function = commandPacket::LoadBackendDispatchFunction(packet);
        const void* command = commandPacket::LoadCommand(packet);
        function(command);
    }
    
    Key* _keys;
    void** _data;
    CommandPacket* _packets;
    unsigned int _current;
};

#endif /* COMMANDSTACK_H */
