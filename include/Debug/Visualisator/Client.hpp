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

#ifndef CLIENT_H
#define CLIENT_H

#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include <map>
#include <utility>

const CL_String SERVER_PORT = "1992";
const CL_String SERVER_HOST = "localhost";

struct watch
{
    std::string id, name, value, parent;

    watch() { }
    watch(std::string id_, std::string name_, std::string value_, std::string parent_)
        : id(id_), name(name_), value(value_), parent(parent_) { }
};

class Client
{
public:
    Client();
    ~Client();

    void connect_to_server();
    void checkEvents();
    void waitEvents();
    void disconnect();

    bool connected;
    std::map<int, watch> mWatchesHandles;
    int mCounter; // How many watches have been added

private:
    void on_connected();
    void on_disconnected();

    void on_event_received(const CL_NetGameEvent &e);

    // Command handlers
    void add(const CL_NetGameEvent &event);
    void update(const CL_NetGameEvent &event);
    void remove(const CL_NetGameEvent &event);

private:
    CL_NetGameClient network_client;
    CL_SlotContainer slots;

    CL_NetGameEventDispatcher_v0 events;
};

#endif // CLIENT_H
