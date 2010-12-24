/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Core/Application.hpp"

int Application::main(const std::vector<CL_String> &args)
{
    LOG_NOFORMAT("\t---------- Loading systems ---------\n");
    // Main environment handlers
    CL_SetupCore coreHandle;
    LOG("Core handle is initialized");
    CL_SetupDisplay displayHandle;
    LOG("Display handle is initialized");
    CL_SetupGL gl2Handle;
    CL_SetupGL1 gl1Handle;
    CL_SetupSWRender swHandle;
    LOG("GL handle is initialized");
    CL_SetupSound soundHandle;
    LOG("Sound handle is initialized");
    CL_SetupVorbis vorbisHandle;
    LOG("Vorbis module is initialized");

    try
    {
        // Set software render as default
        swHandle.set_current();
        std::string render = configManager().getValue<std::string>("window.render", "software");
        LOG("Chosen render is " + render);
        if (render == "opengl1")
            gl1Handle.set_current();
        else if (render == "opengl2")
            gl2Handle.set_current();

        std::string mediaPath = configManager().getValue<std::string>("application.media-folder", "media");
        utils().setMediaFolder(mediaPath);

        appManager().initWindow("Themisto");
        LOG("The window has been created!");

        inputManager().initInput();
        LOG("Input system is configured");

        guiManager().initGui(appManager().getWindow(), "media/gui_basic/");
        LOG("The gui has been initialized!");

        CL_GUIManager gui = guiManager().getHandle();
        CL_GUIWindowManagerTexture wm = guiManager().getWM();
        CL_GraphicContext gc = appManager().getGraphic();

        resourceManager().loadFonts();
        resourceManager().loadTextures();

        CL_Slot slotQuit = appManager().getWindow().sig_window_close().connect(Application::onWindowClose);
        CL_Slot slotInput = inputManager().getKeyboard().sig_key_up().connect(Application::onInput);

        // Queueing the states
        bool physic = false;
        for (unsigned int i=0; i < args.size(); i++)
        {
           if (args[i] == "physic") physic = true;
        }

        if (physic)
           stateManager().push(new PhysicState);
        else
           stateManager().push(new MenuState);

        while (appManager().getRunning())
        {
            appManager().frameStarted();
            stateManager().update();

            wm.process();
            wm.draw_windows(gc);

            appManager().getWindow().flip(1);
            CL_KeepAlive::process(0);
            appManager().frameEnded();
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
        appManager().setRunning(false);
    }
}

void Application::onWindowClose()
{
    appManager().setRunning(false);
}

void Application::doNothing() { }

CL_ClanApplication app(&Application::main);
