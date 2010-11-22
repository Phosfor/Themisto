
#include "include/Debug/Visualisator/Client.h"
//#include <boost/process.hpp>

Client::Client()
{
    events.func_event("Add").set(this, &Client::add);
    events.func_event("Update").set(this, &Client::update);
    events.func_event("Remove").set(this, &Client::remove);

    // Connect essential signals - connecting, disconnecting and receiving events
    slots.connect(network_client.sig_event_received(), this, &Client::on_event_received);
    slots.connect(network_client.sig_connected(), this, &Client::on_connected);
    slots.connect(network_client.sig_disconnected(), this, &Client::on_disconnected);

}

void Client::add(const CL_NetGameEvent &event)
{
    std::string watchID = event.get_argument(0).to_string().c_str();
    std::string watchName = event.get_argument(1).to_string().c_str();
    std::string watchValue = event.get_argument(2).to_string().c_str();
    std::string watchParent = event.get_argument(3).to_string().c_str();
    
    std::cout<< "Add watch ID = " << watchID << " Name = "
        <<  watchName << " Value = " << watchValue << " Parent = " <<watchParent;
}
void Client::update(const CL_NetGameEvent &event)
{
    std::string watchID = event.get_argument(0).to_string().c_str();
    std::string watchValue = event.get_argument(1).to_string().c_str();
    
    std::cout<< "Update watch ID = " << watchID << " New Value = " << watchValue;
}
void Client::remove(const CL_NetGameEvent &event)
{
    std::string watchID = event.get_argument(0).to_string().c_str();
    
    std::cout<< "Remove watch ID = " << watchID;
}

void Client::checkEvents()
{
    network_client.process_events();
}

void Client::waitEvents()
{
    network_client.process_events();
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
    CL_NetGameEvent hello(CL_String("Hello"), CL_String("I am Visualisator"));
    network_client.send_event(hello);
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
