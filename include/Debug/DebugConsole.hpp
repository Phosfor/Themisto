/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

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

#ifndef _DEBUG_CONSOLE_HPP_
#define _DEBUG_CONSOLE_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

#include <ClanLib/core.h>
#include <ClanLib/network.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <readline/readline.h>
#include <readline/history.h>

const CL_String SERVER_PORT = "1992";
const CL_String SERVER_HOST = "localhost";

class Client
{
    public:
        Client();
        ~Client();

        void step(const std::string &command);

        bool connected;
        bool quit;

        void connect_to_server();
        void disconnect();

        void parseCommand(std::string &command);

    private:
        void on_connected();
        void on_disconnected();

        void on_event_received(const CL_NetGameEvent &e);

    private:
        CL_NetGameClient network_client;
        CL_SlotContainer slots;

        CL_NetGameEventDispatcher_v0 login_events;
        CL_NetGameEventDispatcher_v0 game_events;
};

#endif /* _DEBUG_CONSOLE_HPP_ */
