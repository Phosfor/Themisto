#include "Core/LevelManager.hpp"

void LevelManager::init(const std::string &textureName)
{
    std::string path = utils().getMediaFolder() + "/" + textureName;
    if (!boost::filesystem::exists(path)) LOG_NOFORMAT(cl_format("Error: Level texture `%1` doesn't exist!", path));
    mLevelTexture = CL_Image(appManager().getGraphic(), path);
}

void LevelManager::setForegroundTexture(const std::string &resourceName)
{
    mForegroundTexture = resourceName;
    mForeground = true;
}

bool LevelManager::getForegroundEnabled()
{
    return mForeground;
}

std::string LevelManager::getForegroundTexture()
{
    return mForegroundTexture;
}

uint16_t LevelManager::getForegroundSize()
{
    return mForegroundActualSize;
}

void LevelManager::setForegroundSize(uint16_t size)
{
    mForegroundActualSize = size;
}

void LevelManager::update()
{
    mLevelTexture.draw(appManager().getGraphic(), 0, 0);
}
