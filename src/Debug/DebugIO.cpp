/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

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

#include "Debug/DebugIO.hpp"

void DebugIO::init()
{
    mVisualisatorConnection = NULL;
    console_events.func_event("Command").set(this, &DebugIO::commandHandler);
    console_events.func_event("Hello").set(this, &DebugIO::parter);

    try
    {
        mConsoleServer.start(SERVER_HOST, CONSOLE_PORT);
    }
    catch(CL_Exception& e)
    {
        LOG("Torubles with standing debug server");
        LOG(e.what()); 
    }
    slotEventReceived = mConsoleServer.sig_event_received().connect(this, &DebugIO::eventHandler);
    slotDisconnect = mConsoleServer.sig_client_disconnected().connect(this, &DebugIO::on_disconnected);
}

void DebugIO::on_disconnected(CL_NetGameConnection * connection)
{
    if(connection == mVisualisatorConnection)
    {
        mVisualisatorConnection = NULL;
    }
}

void DebugIO::step()
{
    mConsoleServer.process_events();  
}

void DebugIO::eventHandler( CL_NetGameConnection *connection,const CL_NetGameEvent &event)
{
    if(!console_events.dispatch(event, connection))
    {
        LOG("Unhandled event.");
    }
}

void DebugIO::commandHandler(const CL_NetGameEvent &event,  CL_NetGameConnection *connection)
{
    CL_String type = event.get_name();

    if(event.get_argument_count() > 0)
    {
        std::string answer;
        mCommand.invoke((std::string)event.get_argument(0).to_string(), &answer);
        CL_NetGameEvent response(CL_String("Answer"), CL_String(answer));
        connection->send_event(response);
    } 
}


void DebugIO::parter(const CL_NetGameEvent &event,  CL_NetGameConnection *connection)
{
    if(event.get_argument_count() > 0)
    {
        if(event.get_argument(0).is_string())
        {
            if(event.get_argument(0).to_string() == "I am Visualisator")
            {
                mVisualisatorConnection = connection;
            }
        }
    } 
}

void DebugIO::addWatch(std::string id, std::string name, std::string value, std::string parent)
{
    CL_NetGameEvent addWatchRequest(CL_String("Add"), CL_String(id),
         CL_String(name), CL_String(value), CL_String(parent));
    if(mVisualisatorConnection!= NULL)
    {
        try
        {
            mVisualisatorConnection->send_event(addWatchRequest);    
        }
        catch(CL_Exception& e)
        {
            std::string msg = e.what();
            LOG("Error: Can't add watch to visualiser; Reason: " + msg);
        }
    }
    else
    {
        //LOG("Error: visualiser not available.");
        LOG("[" + id + "] " + name + " = " + value);
    }
}
void DebugIO::updateWatch(std::string id, std::string newVal)
{
    CL_NetGameEvent updateWatchRequest(CL_String("Update"), CL_String(id), CL_String(newVal));
    if(mVisualisatorConnection!= NULL)
    {
        try
        {
            mVisualisatorConnection->send_event(updateWatchRequest);  
        }
        catch(CL_Exception& e)
        {
            std::string msg = e.what();
            LOG("Error: Can't update watch in visualiser; Reason: " + msg);
        } 
    }
    else
    {
        //LOG("Error: visualiser not available.");
        LOG("[" + id + "] " + " = " + newVal);
    } 
}
void DebugIO::removeWatch(std::string id)
{
    CL_NetGameEvent removeWatchRequest(CL_String("Remove"), CL_String(id));
    if(mVisualisatorConnection!= NULL)
    {
        try
        {
            mVisualisatorConnection->send_event(removeWatchRequest);  
        }
        catch(CL_Exception& e)
        {
            std::string msg = e.what();
            LOG("Error: Can't remove watch from visualiser; Reason: " + msg);
        }  
     }
    else
    {
        LOG("Error: visualiser not available.");
        LOG("[" + id + "] " + "removed ");
    }  
}
