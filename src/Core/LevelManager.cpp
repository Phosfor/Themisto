/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Core/LevelManager.hpp"
#include "World/Actions/Action.hpp"

LevelManager::LevelManager() :
    mObjectsByIndexViewer(mObjectsSet.get<tag_zindex>()),
    mObjectsByNameViewer(mObjectsSet.get<tag_name>()),
    mObjectsByTypeViewer(mObjectsSet.get<tag_type>())
{
    mNumObjects = 0;
    mCameraSpeed = configManager().getValue<float>("application.camera_speed", 10.0f);
    mDebugDrawOnly = configManager().getValue<bool>("application.debug_draw_only", false);
    mDrawDebugData = mDrawActions = false;
}

// This function walk through all py-files which should consist of
// game objects types. When we run the file, new class registers in
// our script namespace
void LevelManager::processScriptObjects()
{
    namespace bf = boost::filesystem;

    bf::directory_iterator endIt;
    for ( bf::recursive_directory_iterator end, dir(utils().getMediaFolder() + "/objects/");
           dir != end; ++dir )
    {
        if (!bf::is_directory(dir->path()) && bf::extension(dir->path()) == ".py")
                scriptsManager().runFile(dir->path().c_str());
    }
}

void LevelManager::mousePressed(const CL_InputEvent &key, const CL_InputState &state)
{
    // Check if menu is already drawn, maybe we have to hide it?
    if (mDrawActions)
    {
        /*CL_Rectf objRect = mActiveObject->getRectangle();
        CL_Rectf camPos = getAbsoluteCameraPos();
        objRect.expand(50, 50, 0, 0); // left, top, right, bottom

        CL_Point p = key.mouse_pos;
        CL_Rectf col(camPos.left + p.x, camPos.top + p.y, camPos.left + p.x, camPos.top + p.y);
        if (!objRect.is_inside(col))
        {
            clearMenu();
            return;
        }*/
    }

    if (key.id == CL_MOUSE_RIGHT && !mDrawActions)
    {
        /*CL_Point mousePos = key.mouse_pos;

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
        }*/
    }
}

void LevelManager::clearMenu()
{
    /*if (mDrawActions)
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
    }*/
}

void LevelManager::drawActions()
{
    /*uint16_t counter = 0;
    CL_Rectf camPos = getAbsoluteCameraPos();
    BOOST_FOREACH(CL_PushButton *button, mButtons)
    {
        CL_Rectf newPos = mActiveObject->getRectangle();
        button->set_geometry(CL_Rect(newPos.left - 25 - camPos.left, newPos.top + counter - camPos.top, CL_Size(50, 50)));
        counter += 51;
    }*/
}

void LevelManager::menuItemClicked(boost::shared_ptr<Action> clickedAction)
{
    std::cout << "Clicked\n";

    //clearMenu();
}

void LevelManager::initObjects()
{
    BOOST_FOREACH(const ObjectStorage &it, mObjectsByIndexViewer)
    {
        // TODO: !!!!
        // This is called only for objects which are created from level-file
        // When we add object dynamically in game process, this function won't be called!
        // Insert some flag, and if game already runs, init object in addObject function
        it.getCppObject()->init();
    }
    //mActiveActor = getObjectByType<Player>("Player");
}

void LevelManager::init()
{
    mMousePressedSlots = inputManager().getMouse().sig_key_down().connect(&levelManager(), &LevelManager::mousePressed);

    // Yep, maybe we don't need this
    guiManager().getHandle().set_css_document(utils().getMediaFolder() + "/local.css");

    /*boost::python::object levelData = getObjectByType<Level>("Level");
    if (levelData->getType() != "Empty")
    {
        std::vector<std::string> info = levelData->getTextureInfo();
        mTextureSize = resourceManager().getImage(info[0], info[1]).get_size();
    }
    else
    {
        mTextureSize = CL_Size(ScreenResolutionX, ScreenResolutionY);
    }*/
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

void LevelManager::addObject(const std::string &name, const std::string &type, boost::python::object obj)
{
    try
    {
        if (mObjectsByNameViewer.find(name) != mObjectsByNameViewer.end())
        {
            LOG(cl_format("Object with name `%1` already exists!", name).c_str());
            return;
        }

        // Save native c++ pointer to the base Object class first
        boost::shared_ptr<Object> cpp = boost::python::extract< boost::shared_ptr<Object> >(obj);
        int zIndex = cpp->getIndex();

        // And save bp::object, don't forget to increment pointer counter!
        //bp::incref(obj);

        ObjectStorage storage(name, type, zIndex, cpp, obj);
        mObjectsSet.insert(storage);

        mNumObjects++;
    }
    catch(boost::python::error_already_set &e)
    {
        LOG("Something bad has been happened with script system when adding object...");
        PyErr_Print();
    }
}

void LevelManager::updateLogic(float elapsed)
{

    BOOST_FOREACH(const ObjectStorage &it, mObjectsByIndexViewer)
        it.getCppObject()->update(elapsed);
}

void LevelManager::updateVisual(float elapsed)
{
    if (mDebugDrawOnly) return;
    CL_Rectf camPos = getAbsoluteCameraPos();

    BOOST_FOREACH(const ObjectStorage &storage, mObjectsByIndexViewer)
    {
        boost::shared_ptr<Object> it = storage.getCppObject();
        if (!it->getAlwaysDraw())
        {
            CL_Rectf objRect = it->getRectangle();

            if (mDrawDebugData) CL_Draw::box(appManager().getGraphic(),
                    objRect.right - camPos.left, objRect.top - camPos.top,
                    objRect.left - camPos.left, objRect.bottom - camPos.top,
                    CL_Colorf::red);

            // Check whether object is in camera space
            if ( !(
                   objRect.right  - camPos.left < 0                     || // <-
                   objRect.left   - camPos.left > ScreenResolutionX     || // ->
                   objRect.bottom - camPos.top  < 0                     || // up
                   objRect.top    - camPos.top  > ScreenResolutionY        // down
                  )
                )
            {
                CL_Pointf position = it->getPosition();
                it->updateVisual(position.x - camPos.left, position.y - camPos.top);
            }
        }
        else
        {
            CL_Rectf objRect = it->getRectangle();
            if (mDrawDebugData) CL_Draw::box(appManager().getGraphic(), objRect, CL_Colorf::red);
            CL_Pointf position = it->getPosition();
            it->updateVisual(position.x - camPos.left, position.y - camPos.top);
        }
        if (mDrawActions) drawActions();
    }
}
