#include "Core/Application.hpp"
/*
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o
#include <Box2D/Box2D.h>
using namespace boost::posix_time;



   string getTimeDifference(ptime begin)
   {
   	time_duration result = microsec_clock::local_time() - begin;

   	return to_simple_string(result);
   }



void testPhysic()
{
	b2World world = physicManager.getWorld();

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);


	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);




    ptime begin = microsec_clock::local_time();
	for (int32 i = 0; i < 60; ++i)
	{
	    physicManager.step();
		//world.Step(timeStep, velocityIterations, positionIterations);
		//world.ClearForces();
		//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}
	cout<<getTimeDifference(begin);

}
*/
int Application::main(const std::vector<CL_String> &args)
{


    // Main environment handlers
    CL_SetupCore coreHandle;
    LOG("Core handle is initialized");
    CL_SetupDisplay displayHandle;
    LOG("Display handle is initialized");
    CL_SetupGL glHandle;
    LOG("GL handle is initialized");
    CL_SetupSound soundHandle;
    LOG("Sound handle is initialized");
    //CL_SetupVorbis vorbisHandle;
    //LOG("Vorbis module is initialized");

    try
    {
        string mediaPath = configManager.getValue<string>("application.media-folder", "media");
        utils.setMediaFolder(mediaPath);

        appManager.initWindow("Themisto");
        LOG("The window has been created!");

        inputManager.initInput();
        LOG("Input system is configured");

        //guiManager.initGui(appManager.getWindow(), "media/gui_basic/");
        //LOG("The gui has been initialized!");

        //CL_GUIManager gui = guiManager.getHandle();
        //CL_GUIWindowManagerTexture wm = guiManager.getWM();
        CL_GraphicContext gc = appManager.getGraphic();

        resourceManager.loadFonts();

        CL_Slot slotQuit = appManager.getWindow().sig_window_close().connect(Application::onWindowClose);
        CL_Slot slotInput = inputManager.getKeyboard().sig_key_up().connect(Application::onInput);

        // Queueing the states
        bool physic = false;
        for (unsigned int i=0; i < args.size(); i++)
        {
           if (args[i] == "physic") physic = true;
        }

        if (physic)
           stateManager.push(new PhysicState);
        else
           stateManager.push(new MenuState);

        while (appManager.getRunning())
        {
            appManager.frameStarted();
            stateManager.update();

            //wm.process();
            //wm.draw_windows(gc);

            appManager.getWindow().flip(1);
            CL_KeepAlive::process(0);
            appManager.frameEnded();
        }
    }
    catch(CL_Exception &error)
    {
        CL_ConsoleWindow console("Themisto console", 80, 160);
        CL_Console::write_line("Exception happened: " + error.get_message_and_stack_trace());
        console.display_close_message();
        return -1;
    }

    return 0;
}

void Application::onInput(const CL_InputEvent &key, const CL_InputState &state)
{
    if (key.id == CL_KEY_ESCAPE)
    {
        appManager.setRunning(false);
    }
}

void Application::onWindowClose()
{
    appManager.setRunning(false);
}

void Application::doNothing() { }

CL_ClanApplication app(&Application::main);
