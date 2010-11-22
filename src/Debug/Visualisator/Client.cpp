
#include "include/Debug/Visualisator/Client.h"
//#include <boost/process.hpp>

Client::Client()
{
    events.func_event("Add").set(this, &DebugIO::add);
    events.func_event("Update").set(this, &DebugIO::update);
    events.func_event("Remove").set(this, &DebugIO::remove);

    // Connect essential signals - connecting, disconnecting and receiving events
    slots.connect(network_client.sig_event_received(), this, &Client::on_event_received);
    slots.connect(network_client.sig_connected(), this, &Client::on_connected);
    slots.connect(network_client.sig_disconnected(), this, &Client::on_disconnected);

}

void add(const CL_NetGameEvent &event)
{
    string watchID = event.get_argument(0).to_string().c_str();
    string watchName = event.get_argument(1).to_string().c_str();
    string watchValue = event.get_argument(2).to_string().c_str();
    string watchParent = event.get_argument(3).to_string().c_str();
    
    cout<< "Add watch ID = " << watchID << " Name = " 
        <<  watchName << " Value = " << watchValue << " Parent = " <<watchParent;
}
void update(const CL_NetGameEvent &event)
{
    string watchID = event.get_argument(0).to_string().c_str();
    string watchValue = event.get_argument(1).to_string().c_str();
    
    cout<< "Update watch ID = " << watchID << " New Value = " << watchValue;
}
void remove(const CL_NetGameEvent &event)
{
    string watchID = event.get_argument(0).to_string().c_str();
    
    cout<< "Remove watch ID = " << watchID;
}

void Client::checkEvents()
{
    network_client.process_events();
}

void Client::waitEvents()
{
    CL_Event::wait(network_client.get_event_arrived());
    network_client.process_events();
}

Client::~Client() { }

void Client::connect_to_server()
{
    try {
        network_client.connect(SERVER_HOST, SERVER_PORT);
    } catch(const CL_Exception &e) {
        std::cout << "-->Error during connecting to server.";
    }

    while(!connected)
    {
        CL_Event::wait(network_client.get_event_arrived());
        network_client.process_events();
    }
}

void Client::on_connected()
{
    std::cout << "-->Sucessfully connected to the server.\n";
    connected = true;
}

void Client::on_disconnected()
{
    std::cout << "-->Disconnected from server.\n";
    connected = false;
}

void Client::on_event_received(const CL_NetGameEvent &e)
{
    events.dispatch(e);
}

void Client::disconnect()
{
    std::cout << "-->Disconnecting from server...\n";
    network_client.disconnect();
    connected = false;
}
