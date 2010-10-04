#include "Core/Application.hpp"

int Application::main(const std::vector<CL_String> &args)
{
    // Main environment handlers
    CL_SetupCore coreHandle;
    LOG("Core handle is initialized");
    CL_SetupDisplay displayHandle;
    LOG("Display handle is initialized");
    CL_SetupGL glHandle;
    LOG("GL handle is initialized");

    try
    {
        string mediaPath = configManager.getValue<string>("application.media-folder", "media");
        utils.setMediaFolder(mediaPath);

        appManager.initWindow("Themisto");
        LOG("The window has been created!");

        guiManager.initGui(appManager.getWindow(), "media/gui_basic/");
        LOG("The gui has been initialized!");

        CL_GUIManager gui = guiManager.getHandle();
        CL_GraphicContext gc = appManager.getGraphic();
        CL_GUIWindowManagerTexture wm = guiManager.getWM();

        wm.func_repaint().set(Application::doNothing);

        resourceManager.loadFonts();

        CL_Slot slotQuit = appManager.getWindow().sig_window_close().connect(Application::onWindowClose);
        CL_Slot slotInput = appManager.getKeyboard().sig_key_up().connect(Application::onInput);

        // Queueing the states
        stateManager.push(new MenuState);

        while (appManager.getRunning())
        {
            appManager.frameStarted();
            stateManager.update();

            gui.exec(false);
            gui.process_messages(0);
            wm.draw_windows(gc);

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
