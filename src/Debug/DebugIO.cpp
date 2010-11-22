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
        string answer;
        mCommand.invoke((string)event.get_argument(0).to_string(), &answer);
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

void DebugIO::addWatch(string id, string name, string value, string parent)
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
            string msg = e.what();
            LOG("Error: Can't add watch to visualiser; Reason: " + msg);
        }
    }
    else
    {
        //LOG("Error: visualiser not available.");
        LOG("[" + parent + ":" + id + "] " + name + " = " + value);
    }
}
void DebugIO::updateWatch(string id, string newVal)
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
            string msg = e.what();
            LOG("Error: Can't update watch in visualiser; Reason: " + msg);
        } 
    }
    else
    {
        //LOG("Error: visualiser not available.");
        LOG("[" + id + "] " + " = " + newVal);
    } 
}
void DebugIO::removeWatch(string id)
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
            string msg = e.what();
            LOG("Error: Can't remove watch from visualiser; Reason: " + msg);
        }  
     }
    else
    {
        //LOG("Error: visualiser not available.");
        LOG("[" + id + "] " + "removed ");
    }  
}
