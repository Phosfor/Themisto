#ifndef _DEBUG_IO_HPP_
#define _DEBUG_IO_HPP_

#include <ClanLib/core.h>
#include "Core/ApplicationManager.hpp"
#include <ClanLib/network.h>
#include "Core/Utils.hpp"

#define debugIO (DebugIO::get_mutable_instance())
#define debugIOConst (DebugIO::get_const_instance())

#define SERVER_PORT "1992"
#define SERVER_HOST "127.0.0.1"

class DebugIO : public boost::serialization::singleton<DebugIO>
{
    private:
        CL_NetGameServer mServer;
        CL_Slot slotEventReceived;
        CL_SetupCore setup_core;
        CL_SetupNetwork setup_network;
        
        void eventHandler( CL_NetGameConnection * connection,const CL_NetGameEvent &event);
    public:
        void init();
        void addWatch(string id, string name, string value, string parent);
        void updateWatch(string id, string newVal);
        void removeWatch(string id);
        
        //CL_Signal_1<string, string> mCommand;
};


#endif
