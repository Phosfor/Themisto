#include "Core/LevelManager.hpp"

void LevelManager::init(const std::string &textureName)
{
    std::string path = utils().getMediaFolder() + "/" + textureName;
    if (!boost::filesystem::exists(path)) LOG_NOFORMAT(cl_format("Error: Level texture `%1` doesn't exist!", path));
    mLevelTexture = CL_Image(appManager().getGraphic(), path);

    mGC = appManager().getGraphic();
    mCameraSpeed = 10.0f;
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
    mForegroundDelta = mLevelTexture.get_height() - ScreenResolutionY;
    if (mForegroundDelta < 0)
        mCameraViewport.top += abs(mForegroundDelta);
    else
        mForegroundDelta = -1;
}

CL_Rectf LevelManager::getCamViewport()
{
    return mCameraViewport;
}

float LevelManager::getCameraSpeed()
{
    return mCameraSpeed;
}

void LevelManager::setCameraSpeed(float speed)
{
    mCameraSpeed = speed;
}

void LevelManager::translateCamera(float x, float y)
{
    std::cout << "left: " << mCameraViewport.left << "\n";
    std::cout << "top: " << mCameraViewport.top << "\n";
    std::cout << "right: " << mCameraViewport.right << "\n";
    std::cout << "bottom: " << mCameraViewport.bottom << "\n";

    // Check X-moving
    if (mCameraViewport.left + x > 0 ||
        abs(mCameraViewport.left) - x + ScreenResolutionX > mLevelTexture.get_width())
        return;

    // Check Y-moving
    // If the texture is smaller then window height
    if (mForegroundDelta != -1)
    {
        y = 0;
    }
    else
    {
        if (mCameraViewport.top + y > 0 ||
            abs(mCameraViewport.top) - y + ScreenResolutionY > mLevelTexture.get_height())
            return;
    }

    mCameraViewport.translate(x, y);
}

CL_Rectf LevelManager::getAbsoluteCameraPos()
{
    return CL_Rectf(abs(mCameraViewport.left),
                    abs(mCameraViewport.top),
                    abs(mCameraViewport.left) + ScreenResolutionX,
                    abs(mCameraViewport.top) + ScreenResolutionY);
}
