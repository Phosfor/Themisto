#include "Core/LevelManager.hpp"

void LevelManager::init(const std::string &textureName)
{
    std::string path = utils().getMediaFolder() + "/" + textureName;
    if (!boost::filesystem::exists(path)) LOG_NOFORMAT(cl_format("Error: Level texture `%1` doesn't exist!", path));
    mLevelTexture = CL_Image(appManager().getGraphic(), path);

    mGC = appManager().getGraphic();
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

bool LevelManager::getForegroundFixed()
{
    return mFixedForeground;
}

void LevelManager::setForegroundFixed(bool fixed)
{
    mFixedForeground = fixed;
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
    mLevelTexture.draw(mGC, mCameraViewport.left, mCameraViewport.top);
}

void LevelManager::setCamViewport(const CL_Rectf  &viewport)
{
    mCameraViewport = viewport;

    // If level texture height is smaller then window resolution, draw texture
    // At the bottom of the screen
    int delta = mLevelTexture.get_height() - ScreenResolutionY;
    if (delta < 0)
        mCameraViewport.top += abs(delta);
}

CL_Rectf LevelManager::getCamViewport()
{
    return mCameraViewport;
}

void LevelManager::translateCamera(float x, float y)
{
    // Check X-moving
    if (mCameraViewport.left + x > 0 ||
        abs(mCameraViewport.left) - x + ScreenResolutionX > mLevelTexture.get_width())
        return;

    // Check Y-moving
    if (mCameraViewport.top + y > 0 ||
        abs(mCameraViewport.top) - y + ScreenResolutionY > mLevelTexture.get_height())
        return;

    mCameraViewport.translate(x, y);
}
