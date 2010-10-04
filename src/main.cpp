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
        // Initializating part
        short width = configManager.getValue<int>("window.width", 640);
        short height = configManager.getValue<int>("window.height", 480);
        bool fullscreen = configManager.getValue<bool>("window.fullscreen", false);
        string mediaPath = configManager.getValue<string>("application.media-folder", "media");

        utils.setMediaFolder(mediaPath);

        CL_DisplayWindowDescription desc("Themisto");
        desc.set_fullscreen(fullscreen);
        desc.set_size(CL_Size(width, height), false);
        appManager.initWindow(desc);
        LOG("The window has been created!");

        CL_GUIWindowManagerTexture wm(appManager.getWindow());
        guiManager.initGui(wm, "media/gui_basic/");
        wm.func_repaint().set(Application::doNothing);

        CL_GraphicContext gc = appManager.getGraphic();
        CL_GUIManager gui = guiManager.getHandle();

        CL_GUIComponent area(&gui, CL_GUITopLevelDescription());
        CL_PushButton button1(&area);
        button1.set_geometry(CL_Rect(100, 100, 200, 120));
        button1.set_text("Okay!");

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
