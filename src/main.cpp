#include "Core/Application.hpp"

int GameApplication::main(const std::vector<CL_String> &args)
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
        short width = configManager.getValue<int>("window.width", 640);
        short height = configManager.getValue<int>("window.height", 480);
        bool fullscreen = configManager.getValue<bool>("window.fullscreen", false);
        string mediaPath = configManager.getValue<string>("application.media-folder", "media");
        utils.setMediaFolder(mediaPath);

        CL_DisplayWindow window("Themisto", width, height, fullscreen);
        LOG("The window has been created!");

        /*TODO: Load all another resources stuff */
        resourceManager.loadFonts();

        CL_GraphicContext gc = window.get_gc();
        CL_InputDevice keyboard = window.get_ic().get_keyboard();

        CL_Slot slotQuit = window.sig_window_close().connect(GameApplication::onWindowClose);
        CL_Slot slotInput = keyboard.sig_key_up().connect(GameApplication::onInput);

        CL_Font font(gc, "MailRay", 30);

        while (frameManager.getRunning())
        {
            frameManager.frameStarted();
            gc.clear(CL_Colorf::gray);

            font.draw_text(gc, 10, 25, CL_String(cl_format("fps: %1", frameManager.getFps())), CL_Colorf::black);
            font.draw_text(gc, 10, 50, CL_String(cl_format("elapsed: %1", frameManager.getElapsed())), CL_Colorf::black);

            CL_KeepAlive::process();
            /*TODO: wtf! (check vsync) */
            /*
             * CL_Display::flip()   - Locks to the default frame rate
             * CL_Display::flip(-1) - Locks to the default frame rate
             * CL_Display::flip(0)  - Do not lock to the frame rate
             * CL_Display::flip(1)  - Sync to every frame
             * CL_Display::flip(2)  - Sync to every 2nd frame
            */
            window.flip(1);
            CL_System::sleep(10);
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

void GameApplication::onInput(const CL_InputEvent &key, const CL_InputState &state)
{
    if (key.id == CL_KEY_ESCAPE)
    {
        frameManager.setRunning(false);
    }
}

void GameApplication::onWindowClose()
{
    frameManager.setRunning(false);
}

CL_ClanApplication app(&GameApplication::main);
