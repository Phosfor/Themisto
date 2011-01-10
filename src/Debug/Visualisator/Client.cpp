/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Debug/Visualisator/Client.hpp"

Client::Client()
    : mCounter(0)
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

    mWatchesHandles.insert(std::make_pair(mCounter, 
        watch(watchID, watchName, watchValue, watchParent)) );

    std::cout<< "Add watch ID = " << watchID << " Name = "
        <<  watchName << " Value = " << watchValue << " Parent = " << watchParent << "\n";

    mCounter++;
}
void Client::update(const CL_NetGameEvent &event)
{
    std::string watchID = event.get_argument(0).to_string().c_str();
    std::string watchValue = event.get_argument(1).to_string().c_str();

    for (unsigned int i=0; i < mWatchesHandles.size(); ++i)
        if (mWatchesHandles[i].id == watchID)
            mWatchesHandles[i].value = watchValue;
}
void Client::remove(const CL_NetGameEvent &event)
{
    std::string watchID = event.get_argument(0).to_string().c_str();

    for (unsigned int i=0; i < mWatchesHandles.size(); ++i)
        if (mWatchesHandles[i].id == watchID)
            mWatchesHandles.erase(i);

    std::cout<< "Remove watch ID = " << watchID << "\n";
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

Client::~Client() 
{
    mWatchesHandles.clear();
}

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
