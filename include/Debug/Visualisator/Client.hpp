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
    std::map<std::string, watch> mWatchesHandles;
};

#endif // CLIENT_H

