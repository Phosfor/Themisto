/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _DEBUG_IO_HPP_
#define _DEBUG_IO_HPP_

#include <ClanLib/core.h>
#include "Core/ApplicationManager.hpp"
#include <ClanLib/network.h>
#include "Core/Utils.hpp"
#include <queue>
#include "Core/ApplicationManager.hpp"

const CL_String CONSOLE_PORT = "1992";
const CL_String VISUAL_PORT = "1993";
const CL_String SERVER_HOST = "localhost";

typedef std::pair<CL_NetGameConnection*, const CL_NetGameEvent&> EventInfo;

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
        void addWatch(std::string id, std::string name, std::string value, std::string parent);
        void updateWatch(std::string id, std::string newVal);
        void removeWatch(std::string id);

        CL_Signal_v2<std::string, std::string*> mCommand;
};

inline DebugIO &debugIO() { return DebugIO::get_mutable_instance(); }

#endif /* _DEBUG_IO_HPP_ */
