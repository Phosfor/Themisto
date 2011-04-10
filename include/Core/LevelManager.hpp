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

#pragma once

#include <map>
#include <utility>
#include <string>
#include <set>

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include <boost/serialization/singleton.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/cstdint.hpp>

#include "Core/ApplicationManager.hpp"
#include "Core/InputManager.hpp"
#include "Core/GuiManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Utils.hpp"

#include "World/Objects/Object.hpp"
#include "World/Objects/Foreground.hpp"
#include "World/Objects/Level.hpp"
#include "World/Objects/Empty.hpp"
#include "World/Objects/Player.hpp"
#include "World/Objects/Actor.hpp"

// Used for objects map to achive z-index
struct Compare
{
    bool operator()(const std::string &a, const std::string &b) const
    {
        unsigned int index1 = a.find_first_of('_');
        std::string num1 = a.substr(0, index1);
        int num_1 = atoi(num1.c_str());

        unsigned int index2 = b.find_first_of('_');
        std::string num2 = b.substr(0, index2);
        int num_2 = atoi(num2.c_str());

        return num_1 <= num_2;
    }
};

//class Object;
typedef std::map<std::string, boost::python::object> ObjectMapTypeAccess;
typedef std::map<std::string, boost::python::object> ObjectMapTypeSorted;

namespace bmi = boost::multi_index;
typedef boost::multi_index_container<boost::shared_ptr<Object>,
            bmi::indexed_by<
                bmi:: ordered_non_unique<
                    bmi::mem_fun<Object, int, &Object::getIndex>
                >
            >
        > ObjectWrapperSet;

class Action;
class LevelManager : public boost::serialization::singleton<LevelManager>
{
    private:
        // Level stuff
        std::string mLevelName;
        std::string mForegroundTexture;
        int mForegroundDelta;  // If level texture is < then window height
        CL_Size mTextureSize;
        bool mDebugDrawOnly;
        boost::shared_ptr<Actor> mActiveActor;

        // Camera stuff
        float mCameraSpeed;
        CL_Rectf mCameraViewport;

        // Draw bounding boxes of objects?
        bool mDrawDebugData;

        CL_Slot mMousePressedSlots;

        // Objects stuff
        ObjectMapTypeAccess mObjects;
        ObjectMapTypeSorted mObjectsSorted;
        uint16_t mNumObjects;

        std::vector<boost::python::object> mPythonObjects;
        std::vector< boost::shared_ptr<Object> > mCppObjects;

        // Stores boost::shared_ptr<Object>
        ObjectWrapperSet mObjectsSet;
        // Order by ZIndex
        ObjectWrapperSet::nth_index<0>::type &mObjectsByIndexViewer;

    public:
        LevelManager();

        // Level stuff
        std::string getLevelName();
        void setLevelName(const std::string &name);

        // Camera stuff
        void setCamViewport(const CL_Rectf &viewport);
        CL_Rectf getCamViewport();

        void translateCamera(float x, float y);
        CL_Rectf getAbsoluteCameraPos();

        float getCameraSpeed();
        void setCameraSpeed(float speed);

        bool getDrawDebugData();
        void setDrawDebugData(bool draw);
        void mousePressed(const CL_InputEvent &key, const CL_InputState &state);

        void init();
        void initObjects();

        // Objects stuff
        void addObject(const std::string &name, boost::python::object obj);

        // Get object by name
        boost::python::object getObject(const std::string &name)
        {
            if (mObjects.find(name) == mObjects.end())
            {
                LOG(cl_format("Failed to get object `%1`, it doesn't exist!", name));
                //boost::shared_ptr<T> temp(new T);
                //temp->setType("Empty");
                //return temp;
            }
            else
            {
                //return boost::static_pointer_cast<T>(mObjects[name]);
                return mObjects[name];
            }
        }

        // Returns list of all objects by given type
        std::vector<boost::python::object> getObjectsByType(const std::string &type)
        {
            std::vector<boost::python::object> res;
            BOOST_FOREACH(ObjectMapTypeAccess::value_type &pair, mObjects)
            {
                /*if (pair.second->getType() == type)
                    res.push_back(mObjects[pair.first]);*/
            }

            return res;
        }

        // Returns first object by given type
        boost::python::object getObjectByType(const std::string &type)
        {
            /*BOOST_FOREACH(ObjectMapTypeAccess::value_type &pair, mObjects)
            {
                if (pair.second->getType() == type)
                    return mObjects[pair.first];
            }*/

            LOG(cl_format("No one object with `%1` type!", type));
        }

        void updateVisual(float elapsed);
        void updateLogic(float elapsed);

        CL_Point mClickedPos;
        boost::python::object mActiveObject;
        std::vector< boost::shared_ptr<Action> > mActiveActions;
        std::vector<CL_PushButton*> mButtons;
        bool mDrawActions;
        void drawActions();
        void menuItemClicked(boost::shared_ptr<Action> clickedAction);
        void clearMenu();
};

inline LevelManager &levelManager() { return LevelManager::get_mutable_instance(); }
