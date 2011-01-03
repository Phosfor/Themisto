#include "World/Environ/Foreground.hpp"

Foreground::Foreground()
{
    std::string textureName = levelManager().getForegroundTexture();
    mForeground = resourceManager().getImage("Foreground", textureName);
    mGC = appManager().getGraphic();
}

void Foreground::update(float windPower, float elapsed, float globalTime)
{
    mForeground.draw(mGC, 0, 0);
}
