#include "DebugConsole/DebugConsole.hpp"

Client::Client()
{
    // Connect essential signals - connecting, disconnecting and receiving events
    slots.connect(network_client.sig_event_received(), this, &Client::on_event_received);
    slots.connect(network_client.sig_connected(), this, &Client::on_connected);
    slots.connect(network_client.sig_disconnected(), this, &Client::on_disconnected);

    // Set up event dispatchers to route incoming events to functions
    login_events.func_event("Login-Success").set(this, &Client::on_event_login_success);
    login_events.func_event("Login-Fail").set(this, &Client::on_event_login_fail);

    connected = false;
}

Client::~Client() { }

void Client::exec()
{
    std::cout << "->Trying to connect to the main application...\n";

    connect_to_server();
    while(!connected)
    {
        CL_Event::wait(network_client.get_event_arrived());
        network_client.process_events();
    }

    CL_NetGameEvent handshake(CL_String("Hello, I am Console"));
    network_client.send_event(handshake);
    network_client.process_events();
}

void Client::process_net_events(const std::string command)
{
    cout << "Command: " << command << "\n";
    if (command == "-1") return;

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
        cout << "->Error: can't send command to server; Reason: " <<  e.what();
    }
}

void Client::connect_to_server()
{
    try {
        network_client.connect(SERVER_HOST, SERVER_PORT);
    } catch(const CL_Exception &e) {
        cl_log_event("->Error during connecting to server.", e.message);
    }
}

void Client::on_connected()
{
    std::cout << "->Sucessfully connected to the server!\n";
    connected =true;
}

void Client::on_disconnected()
{
    std::cout << "->Disconnecting from server...\n";
}

void Client::on_event_received(const CL_NetGameEvent &e) 
{
    if(e.get_name() == "Answer")
    {
        std::cout << (string)e.get_argument(0).to_string() <<"\n"; 
    }
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

    bool connected = false;
    Client client;

    std::cout << "\t========= Welcome to Themisto DebugConsole =========\n";
    while (true)
    {
        std::string command;
        std::cout << ">";
        std::getline(std::cin, command, '\n');
        boost::to_lower(command);

        if (command == "quit") {
            break;
        } else if (command == "connect") {
            client.exec();
            connected = true;
            continue;
        }

        if (connected)
        {
            client.process_net_events(command);
            continue;
        }
    }

    return 0;
}
