#include "DebugConsole/DebugConsole.hpp"
#include <iostream>

Client::Client()
{
    // Connect essential signals - connecting, disconnecting and receiving events
    slots.connect(network_client.sig_event_received(), this, &Client::on_event_received);
    slots.connect(network_client.sig_connected(), this, &Client::on_connected);
    slots.connect(network_client.sig_disconnected(), this, &Client::on_disconnected);

    // Set up event dispatchers to route incoming events to functions
    login_events.func_event("Login-Success").set(this, &Client::on_event_login_success);
    login_events.func_event("Login-Fail").set(this, &Client::on_event_login_fail);

    quit = false;
}

Client::~Client() { }

void Client::exec()
{
    std::cout << "Trying to connect to the main application...\n";

    connect_to_server();
    while (!quit)
    {
        CL_Event::wait(network_client.get_event_arrived());
        network_client.process_events();
    }
}

void Client::connect_to_server()
{
    try {
        network_client.connect(SERVER_HOST, SERVER_PORT);
    } catch(const CL_Exception &e) {
        cl_log_event("Error during connecting to server.", e.message);
    }
}

void Client::on_connected()
{
    std::cout << "Sucessfully connected to the server!\n";
    // network_client.send_event(CL_NetGameEvent("Login", "my user name"));
}

void Client::on_disconnected()
{
    std::cout << "Disconnecting from server...\n";
    quit = true;
}

void Client::on_event_received(const CL_NetGameEvent &e) 
{
    std::cout << "Received event from server: ";
    std::cout << e.to_string().c_str() << "\n";

    bool handled_event = false;
    handled_event = login_events.dispatch(e);
}

void Client::on_event_login_success(const CL_NetGameEvent &e)
{
    logged_in = true;
    network_client.send_event(CL_NetGameEvent("La-la-la"));
}

void Client::on_event_login_fail(const CL_NetGameEvent &e)
{
    CL_String fail_reason = e.get_argument(0);
}


int main()
{
    CL_SetupCore setup_core;
    CL_SetupNetwork setup_network;

    std::cout << "\t========= Welcome to Themisto DebugConsole =========\n";

    Client client;
    client.exec();

    return 0;
}
