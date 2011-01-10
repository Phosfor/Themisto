#include "World/Environ/Foreground.hpp"

Foreground::Foreground()
{
    std::string textureName = levelManager().getForegroundTexture();
    mForeground = resourceManager().getImage("Foreground", textureName);
    mGC = appManager().getGraphic();
}

void Foreground::update(float elapsed)
{
    float yPos = 0.0f;
    if (!levelManager().getForegroundFixed()) yPos = levelManager().getCamViewport().top;
    mForeground.draw(mGC, levelManager().getCamViewport().left * 0.5, yPos);
}
