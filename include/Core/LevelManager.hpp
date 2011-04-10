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

#include <string>

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

#include "Core/ApplicationManager.hpp"
#include "Core/InputManager.hpp"
#include "Core/ScriptsManager.hpp"
#include "Core/GuiManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Utils.hpp"

#include "World/Objects/Object.hpp"
#include "World/Objects/Actor.hpp"

namespace bmi = boost::multi_index;

// Object storage interface
// Used for multi_index array and single access interface
class ObjectStorage
{
    private:
        std::string objName, type;
        int zIndex;

        // Reference for the Object interface
        boost::shared_ptr<Object> mCppObject;

        // Reference for the Python interface
        boost::python::object mPythonObject;

    public:
        ObjectStorage(const std::string &name, const std::string &type_,
                      int zIndex_, boost::shared_ptr<Object> cpp, boost::python::object python)
            : objName(name), type(type_), zIndex(zIndex_),
              mCppObject(cpp), mPythonObject(python) {}

        std::string getName() const { return objName; };
        std::string getType() const { return type; };
        int getZIndex() const { return zIndex; }

        boost::shared_ptr<Object> getCppObject() const { return mCppObject; }
        boost::python::object getPythonObject() const { return mPythonObject; }
};

// Tagging for multi_index container
struct tag_zindex {};
struct tag_name {};
struct tag_type {};

typedef boost::multi_index_container<ObjectStorage,
            bmi::indexed_by<
                // ZIndex
                bmi::ordered_non_unique<
                    bmi::tag<tag_zindex>,
                    bmi::const_mem_fun<ObjectStorage, int, &ObjectStorage::getZIndex>
                >,

                // Name
                bmi::ordered_unique<
                    bmi::tag<tag_name>,
                    bmi::const_mem_fun<ObjectStorage, std::string, &ObjectStorage::getName>
                >,

                // Type
                bmi::ordered_non_unique<
                    bmi::tag<tag_type>,
                    bmi::const_mem_fun<ObjectStorage, std::string, &ObjectStorage::getType>
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
        uint16_t mNumObjects;

        // Stores boost::shared_ptr<Object>
        ObjectWrapperSet mObjectsSet;
        // Order by ZIndex
        ObjectWrapperSet::index<tag_zindex>::type &mObjectsByIndexViewer;
        ObjectWrapperSet::index<tag_name>::type &mObjectsByNameViewer;
        ObjectWrapperSet::index<tag_type>::type &mObjectsByTypeViewer;

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

        // Get object by name [python]
        boost::python::object getPyObject(const std::string &name)
        {
            if (mObjectsByNameViewer.find(name) == mObjectsByNameViewer.end())
            {
                LOG(cl_format("Failed to get python object `%1`, it doesn't exist!", name));
                return boost::python::object();
            }
            else
            {
                return mObjectsByNameViewer.find(name)->getPythonObject();
            }
        }

        // Get object by name [cpp]
        boost::shared_ptr<Object> getCppObject(const std::string &name)
        {
            if (mObjectsByNameViewer.find(name) == mObjectsByNameViewer.end())
            {
                LOG(cl_format("Failed to get cpp object `%1`, it doesn't exist!", name));
                return boost::shared_ptr<Object>();
            }
            else
            {
                return mObjectsByNameViewer.find(name)->getCppObject();
            }
        }

        // Returns list of all objects by given type [python]
        std::vector<boost::python::object> getPyObjectsByType(const std::string &type)
        {
            std::vector<boost::python::object> res;

            typedef ObjectWrapperSet::index<tag_type>::type::const_iterator ByTypeIt;
            std::pair<ByTypeIt, ByTypeIt> n_it = mObjectsByTypeViewer.equal_range(type);

            for (; n_it.first != n_it.second; ++n_it.first)
                res.push_back(n_it.first->getPythonObject());

            return res;
        }

        // Returns list of all objects by given type [cpp]
        std::vector< boost::shared_ptr<Object> > getCppObjectsByType(const std::string &type)
        {
            std::vector< boost::shared_ptr<Object> > res;

            typedef ObjectWrapperSet::index<tag_type>::type::const_iterator ByTypeIt;
            std::pair<ByTypeIt, ByTypeIt> n_it = mObjectsByTypeViewer.equal_range(type);

            for (; n_it.first != n_it.second; ++n_it.first)
                res.push_back(n_it.first->getCppObject());

            return res;
        }

        // Returns first object by given type [python]
        boost::python::object getPyObjectByType(const std::string &type)
        {
            return mObjectsByTypeViewer.find(type)->getPythonObject();
            LOG(cl_format("No one object with `%1` type!", type));
        }

        // Returns first object by given type [cpp]
        boost::shared_ptr<Object> getCppObjectByType(const std::string &type)
        {
            return mObjectsByTypeViewer.find(type)->getCppObject();
            LOG(cl_format("No one object with `%1` type!", type));
        }

        void updateVisual(float elapsed);
        void updateLogic(float elapsed);

        // This function registers all available python-objects
        void processScriptObjects();

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
