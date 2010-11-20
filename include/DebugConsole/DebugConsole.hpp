#ifndef _CORE_DEBUG_CONSOLE_HPP_
#define _CORE_DEBUG_CONSOLE_HPP_

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/application.h>

class DebugConsole
{
    private:
        CL_ConsoleWindow *mConsoleWindow;

    public:
        ~DebugConsole();
        void initConsoleWindow(const std::string &title, int width = 800, int height = 600);
        CL_ConsoleWindow &getConsoleWindow();
};

#endif /* _CORE_DEBUG_CONSOLE_HPP_ */
