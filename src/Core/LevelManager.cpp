#include "Core/LevelManager.hpp"
#include "World/Objects/Object.hpp"

LevelManager::LevelManager()
{
    mNumObjects = 0;
    mCameraSpeed = 10.0f;
}

void LevelManager::init(const std::string &textureName)
{
    std::string path = utils().getMediaFolder() + "/" + textureName;
    if (!boost::filesystem::exists(path)) LOG_NOFORMAT(cl_format("Error: Level texture `%1` doesn't exist!", path));
    mTextureSize = CL_Image(appManager().getGraphic(), path).get_size();
}

void LevelManager::setForegroundTexture(const std::string &resourceName)
{
    mForegroundTexture = resourceName;
    mForeground = true;
}

std::string LevelManager::getForegroundTexture()
{
    return mForegroundTexture;
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

uint16_t LevelManager::getForegroundSize()
{
    return mForegroundActualSize;
}

void LevelManager::setForegroundSize(uint16_t size)
{
    mForegroundActualSize = size;
}

void LevelManager::setCamViewport(const CL_Rectf  &viewport)
{
    mCameraViewport = viewport;

    // If level texture height is smaller then window resolution, draw texture
    // At the bottom of the screen
    mForegroundDelta = mTextureSize.height - ScreenResolutionY;
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
    // Check X-moving
    if (mCameraViewport.left + x > 0 ||
        abs(mCameraViewport.left) - x + ScreenResolutionX > mTextureSize.width)
        return;

    // Check Y-moving
    // If the texture is smaller then window height
    if (mForegroundDelta != -1)
    {
        // If it's smaller, we don't need in any Y axis moving
        y = 0;
    }
    else
    {
        if (mCameraViewport.top + y > 0 ||
            abs(mCameraViewport.top) - y + ScreenResolutionY > mTextureSize.height)
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

void LevelManager::setLevelName(const std::string &name)
{
    mLevelName = name;
}

std::string LevelManager::getLevelName()
{
    return mLevelName;
}

void LevelManager::addObject(const std::string &name, boost::shared_ptr<Object> obj)
{
    if (mObjects.find(name) == mObjects.end())
    {
        mObjects.insert(std::make_pair(name, obj));
        mObjectsSorted.insert(std::make_pair(IntToStr(obj->getIndex()) + "_" + name, obj));
        mNumObjects++;
    }
    else
    {
        LOG(cl_format("Object with name `%1` already exists!", name));
    }
}

void LevelManager::update(float elapsed)
{
    CL_Rectf camPos = getAbsoluteCameraPos();

    for (ObjectMapTypeSorted::const_iterator it=mObjectsSorted.begin();
            it != mObjectsSorted.end(); ++it)
    {
        CL_Rectf objRect = it->second->getRectangle();

        if (camPos.is_overlapped(objRect))
        {
            CL_Pointf position = it->second->getPosition();
            it->second->updateVisual(position.x - camPos.left, position.y - camPos.top);
        }

        it->second->update(elapsed);
    }
}
