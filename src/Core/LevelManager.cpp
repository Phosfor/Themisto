#include "Core/LevelManager.hpp"

LevelManager::LevelManager()
{
    mNumObjects = 0;
    mCameraSpeed = configManager().getValue<float>("application.camera_speed", 10.0f);
    mDebugDrawOnly = configManager().getValue<bool>("application.debug_draw_only", false);
    mDrawDebugData = false;
}

void LevelManager::initObjects()
{
    for (ObjectMapTypeSorted::const_iterator it=mObjectsSorted.begin();
            it != mObjectsSorted.end(); ++it)
    {
        it->second->init();
    }
    mActiveActor = getObjectByType<Player>("Player");
}

void LevelManager::init()
{
    boost::shared_ptr<Foreground> foregroundData = getObjectByType<Foreground>("Foreground");
    if (foregroundData->getType() != "Empty")
    {
        // TODO: Get from level...
        //std::vector<std::string> info = foregroundData->getTextureInfo();
        //mTextureSize = resourceManager().getImage(info[0], info[1]).get_size();
        mTextureSize = resourceManager().getImage("Levels", "test").get_size();
    }
    else
    {
        mTextureSize = CL_Size(ScreenResolutionX, ScreenResolutionY);
    }
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

void LevelManager::setDrawDebugData(bool draw)
{
    mDrawDebugData = draw;
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

void LevelManager::updateLogic(float elapsed)
{
    for (ObjectMapTypeSorted::const_iterator it=mObjectsSorted.begin();
            it != mObjectsSorted.end(); ++it)
    {
        it->second->update(elapsed);
    }
}

void LevelManager::updateVisual(float elapsed)
{
    CL_Rectf camPos = getAbsoluteCameraPos();

    for (ObjectMapTypeSorted::const_iterator it=mObjectsSorted.begin();
            it != mObjectsSorted.end(); ++it)
    {
        if(!mDebugDrawOnly)
        {
            if (!it->second->getAlwaysDraw())
            {
                CL_Rectf objRect = it->second->getRectangle();

                if (mDrawDebugData) CL_Draw::box(appManager().getGraphic(),
                        objRect.right - camPos.left, objRect.top - camPos.top,
                        objRect.left - camPos.left, objRect.bottom - camPos.top,
                        CL_Colorf::red);

                // Check whether object is in camera space
                if ( !(
                       objRect.right - camPos.left < 0                     || // <-
                       objRect.left  - camPos.left > ScreenResolutionX     || //  ->
                       objRect.bottom - camPos.top  < 0                    || // up
                       objRect.top - camPos.top > ScreenResolutionY           // down
                      )
                    )
                {
                    CL_Pointf position = it->second->getPosition();
                    it->second->updateVisual(position.x - camPos.left, position.y - camPos.top);
                }
            }
            else
            {
                CL_Rectf objRect = it->second->getRectangle();
                if (mDrawDebugData) CL_Draw::box(appManager().getGraphic(), objRect, CL_Colorf::red);
                CL_Pointf position = it->second->getPosition();
                it->second->updateVisual(position.x - camPos.left, position.y - camPos.top);
            }
        }
    }
}
