/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include <map>
#include <utility>

const static CL_String SERVER_PORT = "1992";
const static CL_String SERVER_HOST = "localhost";

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

