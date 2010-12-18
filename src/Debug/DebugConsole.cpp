/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Debug/DebugConsole.hpp"
#include <stdio.h>

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

void Client::step(const std::string &command)
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
        std::cout << "# Error: can't send command to Themisto; Reason: " <<  e.what();
    }
}

void Client::connect_to_server()
{
    network_client.process_events();
    try {
        network_client.connect(SERVER_HOST, SERVER_PORT);
    } catch(const CL_Exception &e) {
        cl_log_event("# Error during connecting to Themisto.", e.message);
    }

    while(!connected)
    {
        CL_Event::wait(network_client.get_event_arrived());
        network_client.process_events();
    }
}

void Client::on_connected()
{
    std::cout << "# Sucessfully connected to the Themisto.\n";
    connected = true;
}

void Client::on_disconnected()
{
    std::cout << "# Disconnected from Themisto.\n";
    connected = false;
}

void Client::on_event_received(const CL_NetGameEvent &e) 
{
    if(e.get_name() == "Answer")
    {
        std::string answer = (std::string)e.get_argument(0).to_string(); 
        if( *(answer.rbegin()) != '\n') answer += "\n";
        boost::replace_all(answer, "\n", "\n# ");
        std::size_t posLastEndl = answer.rfind("\n# ");
        if(posLastEndl != answer.npos)
        {
            answer.erase(posLastEndl, answer.npos);
        }
        std::cout << "# " << answer << std::endl;
    }
}

void Client::disconnect()
{
    std::cout << "# Disconnecting from Themisto...\n";
    network_client.disconnect();
    connected = false;
}

// A static variable for holding the line. 
static char *line_read = (char *)NULL;

// Read a string, and return a pointer to it.
// Returns NULL on EOF. 
char* readCommand()
{
    // If the buffer has already been allocated,
    // return the memory to the free pool. 
    if (line_read)
    {
      free (line_read);
      line_read = (char *)NULL;
    }

    // Get a line from the user. */
    line_read = readline (">");

    // If the line has any text in it,
    // save it on the history. 
    if (line_read && *line_read)
      add_history (line_read);

    return (line_read);
}

std::string exec(const std::string &cmd, bool read) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    //std::string result = "";
    if(read)
    {
        while(!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL)
                    std::cout << buffer;
        }
    }
    pclose(pipe);
    return "";
}

void Client::parseCommand(std::string &command)
{
    if ((command == "disconnect") && connected)
    {
        disconnect();
    }
    else if((command == "connect") && !connected)
    {
        connect_to_server();
    }
    else if(command == "rv")
    {
        exec("./DebugVisualisator &", false);
    }
    else if(command == "run" || command == "r")
    {
        exec("./Themisto physic &", false);
    }
    else if(command == "run console" || command == "rc")
    {
        system("gnome-terminal -e './Themisto physic&' &");
    }
    else if(command == "run here" || command == "rh")
    {
        system("./Themisto physic");
    }
    else if(command == "build" || command == "b" || command == "make")
    {
        system("make --quiet -C ../build &");
        system("cd ../bin");
    }
    else if(command.find("git") != command.npos)
    {
        command.insert(0, "cd .. && ");
        exec(command, true);
    }
    else if(command[0] == '.')
    {
        command[0] = ' ';
        system(command.c_str());
    }
    else if(command == "clear")
    {
        system("clear");
    }
    else if(command[0] == '!')
    {
        command[0] = ' ';
        if(command.size() > 1)
        {
            exec("gnome-terminal -e " + command, false);
        }
        else
        {
            exec("gnome-terminal", false);
        }
    }
    else
    {
        if (!connected)
        {
            connect_to_server();
        }
        std::cout << "# Themisto says:\n";
        if (connected)
        {
            step(command);
        }
    }
}

int main(int argc, char* argv[])
{
    CL_SetupCore setup_core;
    CL_SetupNetwork setup_network;

    Client client;
    std::string _command = "";

    for(int i=1; i<argc; ++i)
    {
        _command.append(argv[i]);
        _command.append(" ");
    }

    std::cout << "\t========= Welcome to Themisto DebugConsole =========\n";

    while (!client.quit)
    {
        std::vector<std::string> subcommands;
        boost::split(subcommands, _command, boost::is_any_of("&&"));
        BOOST_FOREACH(std::string command, subcommands)
        {
            boost::trim(command);

            if( command.find("execute") != command.npos || command.find("exe") != command.npos)
            {
                std::size_t paramPos = command.find(" ");
                if(paramPos != command.npos)
                {
                    std::string file = command.substr(paramPos+1, command.npos);
                    if(boost::filesystem::exists(file))
                    {
                        std::ifstream fs;
                        fs.open(file.c_str(), std::ifstream::in);
                        std::string line;
                        char bufer[256];
                        while(fs.good() && !fs.eof())
                        {
                            fs.getline(bufer, 256, '\n');
                            line.assign(bufer);
                            boost::trim(line);
                            if(line != "")
                            {
                                std::cout << ">" << line << std::endl;
                                add_history(line.c_str());
                                client.parseCommand(line);
                            }
                        }
                        fs.close();
                    }
                    else
                    {
                        std::cout << "# Error: file '" + file + "' not exists.\n";
                    }
                }
                else
                {
                    std::cout<< "# Error: script-file not specified.\n";
                }
            }
            else if (command == "q" || command == "quit")
            {
                client.quit = true;
            }
            else if (command != "") 
            {
                client.parseCommand(command);
            }
        }
        if(!client.quit)
        {
            _command = readCommand();
        }
    }

    return 0;
}
