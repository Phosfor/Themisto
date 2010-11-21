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

    quit = false;
    connected = false;
}

Client::~Client() { }

void Client::exec()
{
    std::cout << "Trying to connect to the main application...\n";

    connect_to_server();
    while(!connected)
    {
        CL_Event::wait(network_client.get_event_arrived());
        network_client.process_events();
    }
    while (!quit)
    {
        //cout<< "I'm ready for commands\n";
        string command;
        cin>>command;
        if(command == "quit")
        {
            quit = true;
        }
        else
        {
            CL_NetGameEvent commandNotice(CL_String("Command"));
            commandNotice.add_argument(CL_String(command));
            try
            {
                //cout<< "Sending command...\n";
                network_client.send_event(commandNotice);
                //cout<< "Command sent, wait answer... \n";
                CL_Event::wait(network_client.get_event_arrived());
                network_client.process_events();
            }
            catch(CL_Exception& e)
            {
                cout << "Error: can't send command to server; Reason: " <<  e.what();
            }
        }
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
    connected =true;
    // network_client.send_event(CL_NetGameEvent("Login", "my user name"));
}

void Client::on_disconnected()
{
    std::cout << "Disconnecting from server...\n";
    quit = true;
}

void Client::on_event_received(const CL_NetGameEvent &e) 
{
    //std::cout << "Received event from server: ";
    if(e.get_name() == "Answer")
    {
        std::cout << (string)e.get_argument(0).to_string() <<"\n"; 
    }
    //bool handled_event = false;
    //handled_event = login_events.dispatch(e);
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
