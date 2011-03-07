#include "Core/LevelManager.hpp"
#include "World/Actions/Action.hpp"

LevelManager::LevelManager()
{
    mNumObjects = 0;
    mCameraSpeed = configManager().getValue<float>("application.camera_speed", 10.0f);
    mDebugDrawOnly = configManager().getValue<bool>("application.debug_draw_only", false);
    mDrawDebugData = mDrawActions = false;
}

void LevelManager::mousePressed(const CL_InputEvent &key, const CL_InputState &state)
{
    // Check if menu is already drawn, maybe we have to hide it?
    if (mDrawActions)
    {
        CL_Rectf objRect = mActiveObject->getRectangle();
        CL_Rectf camPos = getAbsoluteCameraPos();
        objRect.expand(50, 50, 0, 0); // left, top, right, bottom

        CL_Point p = key.mouse_pos;
        CL_Rectf col(camPos.left + p.x, camPos.top + p.y, camPos.left + p.x, camPos.top + p.y);
        if (!objRect.is_inside(col))
        {
            clearMenu();
            return;
        }
    }

    if (key.id == CL_MOUSE_RIGHT && !mDrawActions)
    {
        CL_Point mousePos = key.mouse_pos;

        // If right key pressed - check all objects collision with it
        for (ObjectMapTypeSorted::const_iterator it=mObjectsSorted.begin();
        it != mObjectsSorted.end(); ++it)
        {
            // Collision is detected
            if (it->second->checkCollision(CL_Pointf(mousePos.x, mousePos.y)))
            {
                std::cout << "Draw menu here... Object collided: " << it->second->getName() << "\n";
                std::vector< boost::shared_ptr<Action> > actions = it->second->getAvailableActions();
                if (!actions.empty())
                {
                    mActiveActions = actions;
                    mActiveObject = it->second;
                    mClickedPos = mousePos;
                    mDrawActions = true;

                    // Init GUI part
                    CL_Rect clientArea = appManager().getGraphic().get_cliprect();

                    BOOST_FOREACH(boost::shared_ptr<Action> &act, mActiveActions)
                    {
                        CL_PushButton *temp = new CL_PushButton(&guiManager().getWrapper());
                        CL_Rectf newPos = mActiveObject->getRectangle();

                        // We hide background image there
                        temp->set_class_name("action-icon");

                        std::vector<std::string> textureInfo = act->getTextureInfo();
                        temp->set_icon(resourceManager().getImage(textureInfo[0], textureInfo[1]));

                        // Process click event
                        temp->func_clicked().set(this, &LevelManager::menuItemClicked, act);

                        mButtons.push_back(temp);
                    }
                }
                break;
            }
        }
    }
}

void LevelManager::clearMenu()
{
    if (mDrawActions)
    {
        BOOST_FOREACH(CL_PushButton *button, mButtons)
        {
            if (button)
            {
                button->set_visible(false);
                delete button;
            }
        }

        mButtons.clear();
        mActiveActions.clear();
        mActiveObject.reset();
        mDrawActions = false;
    }
}

void LevelManager::drawActions()
{
    uint16_t counter = 0;
    CL_Rectf camPos = getAbsoluteCameraPos();
    BOOST_FOREACH(CL_PushButton *button, mButtons)
    {
        CL_Rectf newPos = mActiveObject->getRectangle();
        button->set_geometry(CL_Rect(newPos.left - 25 - camPos.left, newPos.top + counter - camPos.top, CL_Size(50, 50)));
        counter += 51;
    }
}

void LevelManager::menuItemClicked(boost::shared_ptr<Action> clickedAction)
{
    std::cout << "Clicked\n";

    clearMenu();
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
    mMousePressedSlots = inputManager().getMouse().sig_key_down().connect(&levelManager(), &LevelManager::mousePressed);

    // Yep, maybe we don't need this
    guiManager().getHandle().set_css_document(utils().getMediaFolder() + "/local.css");

    boost::shared_ptr<Level> levelData = getObjectByType<Level>("Level");
    if (levelData->getType() != "Empty")
    {
        std::vector<std::string> info = levelData->getTextureInfo();
        mTextureSize = resourceManager().getImage(info[0], info[1]).get_size();
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

bool LevelManager::getDrawDebugData()
{
    return mDrawDebugData;
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
            if (mDrawActions) drawActions();
        }
    }
}
