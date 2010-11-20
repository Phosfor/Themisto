#include "DebugConsole/DebugConsole.hpp"

DebugConsole::~DebugConsole()
{
    delete mConsoleWindow;
}

void DebugConsole::initConsoleWindow(const std::string &title, int width, int height)
{
    mConsoleWindow = new CL_ConsoleWindow(title);
}
