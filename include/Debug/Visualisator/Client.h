#ifndef CLIENT_H
#define CLIENT_H

#undef signals
#undef slots

#include <ClanLib/core.h>
#include <ClanLib/network.h>

const static CL_String SERVER_PORT = "1992";
const static CL_String SERVER_HOST = "localhost";

class Client
{
public:
    Client();
    ~Client();

    void connect_to_server();
    void checkEvents();
    void waitEvents();
    void disconnect();

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
