#include "DebugConsole/DebugIO.hpp"

void DebugIO::init()
{
    try
    {
        mServer.start(SERVER_HOST, SERVER_PORT);
    }
    catch(CL_Exception& e)
    {
        LOG(e.what()); 
    }
    slotEventReceived = mServer.sig_event_received().connect(this, &DebugIO::eventHandler);
}
void DebugIO::step()
{
    mServer.process_events();
}

void DebugIO::eventHandler( CL_NetGameConnection *connection,const CL_NetGameEvent &event)
{
    string type = event.get_name();
    if( type == "Command")
    {
        if(event.get_argument_count() > 0)
        {
            //string answer = mCommand.invoke(event.get_argument(0));
            //CL_NetGameEvent response("Answer", answer);
            
			//connection->send_event(response);
        }
        
    }
}

void DebugIO::addWatch(string id, string name, string value, string parent)
{
}
void DebugIO::updateWatch(string id, string newVal)
{
}
void DebugIO::removeWatch(string id)
{
}
