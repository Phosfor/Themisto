#ifndef _DEBUG_IO_HPP_
#define _DEBUG_IO_HPP_

#include <ClanLib/core.h>
#include "Core/ApplicationManager.hpp"
#include <ClanLib/network.h>
#include "Core/Utils.hpp"
#include <queue>
#include "Core/ApplicationManager.hpp"


#define debugIO (DebugIO::get_mutable_instance())
#define debugIOConst (DebugIO::get_const_instance())

#define CONSOLE_PORT "1992"
#define VISUAL_PORT "1993"
#define SERVER_HOST "localhost"

typedef pair<CL_NetGameConnection*,const CL_NetGameEvent&> EventInfo;

class DebugIO : public boost::serialization::singleton<DebugIO>
{
    private:
        CL_NetGameServer mConsoleServer;
        CL_NetGameConnection *mVisualisatorConnection;
        CL_Slot slotEventReceived;
        CL_Slot slotDisconnect;
        CL_SetupCore setup_core;
        CL_SetupNetwork setup_network;
        CL_Mutex mMutex;
        std::queue< EventInfo > mReceivedEvents;
        CL_NetGameEventDispatcher_v1<CL_NetGameConnection *> console_events;
        
        void mainLoop();
        void eventHandler( CL_NetGameConnection * connection,const CL_NetGameEvent &event);
        void commandHandler(const CL_NetGameEvent &event, CL_NetGameConnection * connection);
        void parter(const CL_NetGameEvent &event, CL_NetGameConnection * connection);
        void on_disconnected(CL_NetGameConnection * connection);
    public:
        void init();
        void step();
        void addWatch(string id, string name, string value, string parent);
        void updateWatch(string id, string newVal);
        void removeWatch(string id);
        
        CL_Signal_v2<string, string*> mCommand;
};


#endif
