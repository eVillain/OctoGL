#ifndef COMMANDSTACK_H
#define COMMANDSTACK_H

#include "CommandPacket.h"
#include <cstdlib>

namespace DrudgeGL
{
    template <typename T>
    class CommandStack
    {
    public:
        typedef T Key;
        
        CommandStack(const uint32_t size)
        {
            
            m_keys = new Key[size];
            m_data = (void**)malloc(size*sizeof(void*));
            
            m_packets = new CommandPacket[size];
            m_current = 0;
        }
        
        
        ~CommandStack()
        {
            delete [] m_keys;
            delete [] m_data;
            delete [] m_packets;
        }
        
        
        template <typename U>
        U* AddCommand(Key key, size_t auxMemorySize)
        {
            CommandPacket packet = commandPacket::Create<U>(auxMemorySize);
            
            // store key and pointer to the data
            {
                // TODO: add some kind of lock or atomic operation here
                const unsigned int current = m_current++;
                m_keys[current] = key;
                m_packets[current] = packet;
            }
            
            commandPacket::StoreNextCommandPacket(packet, nullptr);
            commandPacket::StoreBackendDispatchFunction(packet, U::DISPATCH_FUNCTION);
            
            return commandPacket::GetCommand<U>(packet);
        }
        
        void Submit(void)
        {
            
            for (unsigned int i=0; i < m_current; ++i)
            {
                CommandPacket packet = m_packets[i];
                do
                {
                    SubmitPacket(packet);
                    packet = commandPacket::LoadNextCommandPacket(packet);
                } while (packet != nullptr);
            }
        }
        
    private:
        void SubmitPacket(const CommandPacket packet)
        {
            const BackendDispatchFunction function = commandPacket::LoadBackendDispatchFunction(packet);
            const void* command = commandPacket::LoadCommand(packet);
            function(command);
        }
        
        Key* m_keys;
        void** m_data;
        CommandPacket* m_packets;
        unsigned int m_current;
    };
}

#endif /* COMMANDSTACK_H */
