#include "DebugConsole/DebugConsole.hpp"


Client::Client()
{
    // Connect essential signals - connecting, disconnecting and receiving events
    slots.connect(network_client.sig_event_received(), this, &Client::on_event_received);
    slots.connect(network_client.sig_connected(), this, &Client::on_connected);
    slots.connect(network_client.sig_disconnected(), this, &Client::on_disconnected);

    quit = false;
    connected = false;
}

Client::~Client() { }

void Client::step(const std::string command)
{
    CL_NetGameEvent commandNotice(CL_String("Command"));
    commandNotice.add_argument(CL_String(command));
    try
    {
        network_client.send_event(commandNotice);
        CL_Event::wait(network_client.get_event_arrived());
        network_client.process_events();
    }
    catch(CL_Exception& e)
    {
        cout << "-->Error: can't send command to server; Reason: " <<  e.what();
    }
}

void Client::connect_to_server()
{
    std::cout << "-->Trying to connect to the main application...\n";

    try {
        network_client.connect(SERVER_HOST, SERVER_PORT);
    } catch(const CL_Exception &e) {
        cl_log_event("-->Error during connecting to server.", e.message);
    }

    while(!connected)
    {
        CL_Event::wait(network_client.get_event_arrived());
        network_client.process_events();
    }
}

void Client::on_connected()
{
    std::cout << "-->Sucessfully connected to the server!\n";
    connected = true;
}

void Client::on_disconnected()
{
    std::cout << "-->Disconnecting from server...\n";
    quit = true;
}

void Client::on_event_received(const CL_NetGameEvent &e) 
{
    if(e.get_name() == "Answer")
        std::cout << "-->" << (string)e.get_argument(0).to_string() <<"\n"; 
}

int main()
{
    CL_SetupCore setup_core;
    CL_SetupNetwork setup_network;

    std::cout << "\t========= Welcome to Themisto DebugConsole =========\n";

    Client client;
    std::string command;

    while (!client.quit)
    {
        cout << ">";
        std::getline(std::cin, command, '\n');
        boost::to_lower(command);

        if (command == "quit")
        {
            client.quit = true;
            continue;
        }
        else if (command == "connect" && !client.connected)
        {
            client.connect_to_server();
            continue;
        }

        if (client.connected)
        {
            client.step(command);
        }
    }

    return 0;
}
