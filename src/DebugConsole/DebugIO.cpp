#include "DebugConsole/DebugIO.hpp"

void DebugIO::init()
{
    try
    {
        mConsoleServer.start(SERVER_HOST, CONSOLE_PORT);
        //mVisualServer.start(SERVER_HOST, VISUAL_PORT);
    }
    catch(CL_Exception& e)
    {
        LOG(e.what()); 
    }
    slotEventReceived = mConsoleServer.sig_event_received().connect(this, &DebugIO::asyncEventHandler);
}

void DebugIO::mainLoop()
{
	while(appManager.getRunning())
	{
		// Wait till we receive any network events
		CL_Event::wait(mConsoleServer.get_event_arrived());
		
	}

	mConsoleServer.stop();
}

void DebugIO::step()
{
    mConsoleServer.process_events();
    CL_MutexSection mutex_lock(&mMutex);
    while(mReceivedEvents.size() > 0)
    {
        EventInfo eventInfo = mReceivedEvents.front();
        mReceivedEvents.pop();
        CL_NetGameConnection *connection = eventInfo.first;
        const CL_NetGameEvent &event = eventInfo.second;
        
        CL_String type = event.get_name();
        cout<< "\n Count: " << event.get_argument_count() << "\n";
        if( type == "Command")
        {
            if(event.get_argument_count() > 0)
            {
                string answer;
                cout<< "\n First is string: " << event.get_argument(0).is_string() << "\n";
                
                mCommand.invoke((string)event.get_argument(0).to_string(), &answer);
                CL_NetGameEvent response(CL_String("Answer"), CL_String(answer));
			    connection->send_event(response);
            }
            
        }
    }
}

void DebugIO::asyncEventHandler( CL_NetGameConnection *connection,const CL_NetGameEvent &event)
{
    CL_MutexSection mutex_lock(&mMutex);
    mReceivedEvents.push(EventInfo(connection, event));
}

void DebugIO::addWatch(string id, string name, string value, string parent)
{
    CL_NetGameEvent addWatchRequest(CL_String("Add"), CL_String(id),
         CL_String(name), CL_String(value), CL_String(parent));
    try
    {
        mVisualServer.send_event(addWatchRequest);    
    }
    catch(CL_Exception& e)
    {
        string msg = e.what();
        LOG("Error: Can't add watch to visualiser; Reason: " + msg);
    }
}
void DebugIO::updateWatch(string id, string newVal)
{
    CL_NetGameEvent updateWatchRequest(CL_String("Update"), CL_String(id), CL_String(newVal));
    try
    {
        mVisualServer.send_event(updateWatchRequest);  
    }
    catch(CL_Exception& e)
    {
        string msg = e.what();
        LOG("Error: Can't update watch in visualiser; Reason: " + msg);
    }  
}
void DebugIO::removeWatch(string id)
{
    CL_NetGameEvent removeWatchRequest(CL_String("Remove"), CL_String(id));
    try
    {
        mVisualServer.send_event(removeWatchRequest);  
    }
    catch(CL_Exception& e)
    {
        string msg = e.what();
        LOG("Error: Can't remove watch from visualiser; Reason: " + msg);
    }    
}
