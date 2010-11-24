#include "Debug/DebugConsole.hpp"
//#include <boost/process.hpp>

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
    network_client.process_events();
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
    if(e.get_name() == "Answer")
        std::cout << "-->" << (string)e.get_argument(0).to_string() <<"\n"; 
}

void Client::disconnect()
{
    std::cout << "-->Disconnecting from server...\n";
    network_client.disconnect();
    connected = false;
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

        command = readline(">");
        if( command != "" ) 
        {
            add_history(command.c_str());
        }

        if (command == "quit" || command == "q")
        {
            client.quit = true;
        }
        else if ((command == "disconnect") && client.connected)
        {
            client.disconnect();
        }
        else if((command == "connect") && !client.connected)
        {
            client.connect_to_server();
        }
        else if(command == "build" || command == "b")
        {
            //namespace bp = ::boost::processes;
            /*bp::command_line buildCMD("make");
            buildCMD.argument("-C");
            buildCMD.argument("build/");

            bp::launcher buildLauncher;
            buildLauncher.set_stdout_behavior(bp::redirect_stream);
            buildLauncher.set_merge_out_err(true);
            buildLauncher.set_work_directory("../"); // Themisto root

            bp::child buildProcess = buildLauncher.start(buildCMD);

            bp::status s = c.wait();
            if (s.exited() && s.exit_status() == EXIT_SUCCESS)
                std::cout << "Directory updated successfully." << std::endl;
            else
                std::cout << "Update failed." << std::endl;*/
        }
        //...else if Other commands, that not need connection
        else
        {
            if (!client.connected) //(command == "connect" || command == "c") &&
            {
                client.connect_to_server();
            }
            if (client.connected)
            {
                client.step(command);
            }
        }
    }

    return 0;
}
