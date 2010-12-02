/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _DEBUG_CONSOLE_HPP_
#define _DEBUG_CONSOLE_HPP_

#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <readline/readline.h>
#include <readline/history.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <boost/foreach.hpp>

const static CL_String SERVER_PORT = "1992";
const static CL_String SERVER_HOST = "localhost";

using namespace std;

class Client
{
public:
    Client();
    ~Client();

    void step(const std::string command);

    bool connected;
    bool quit;

    void connect_to_server();
    void disconnect();
    
    void parseCommand(std::string command);

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
