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

#ifndef _CORE_LEVEL_MANAGER_HPP_
#define _CORE_LEVEL_MANAGER_HPP_

#include <map>
#include <utility>
#include <string>

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include <boost/serialization/singleton.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/cstdint.hpp>

#include "Core/ApplicationManager.hpp"
#include "Core/Utils.hpp"

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

class Object;
typedef std::map<std::string, boost::shared_ptr<Object> > ObjectMapTypeAccess;
typedef std::map<std::string, boost::shared_ptr<Object>, Compare> ObjectMapTypeSorted;

class LevelManager : public boost::serialization::singleton<LevelManager>
{
    private:
        // Level stuff
        std::string mLevelName;
        std::string mForegroundTexture;
        bool mForeground;      // Is foreground image enabled
        bool mFixedForeground; // If foreground image shouldn't move with camera
        int mForegroundDelta;  // If level texture is < then window height
        // Average real foreground image size refer to full window size (in percents)
        uint16_t mForegroundActualSize;
        CL_Size mTextureSize;
        bool mDebugDrawOnly;

        // Camera stuff
        float mCameraSpeed;
        CL_Rectf mCameraViewport;

        // Draw bounding boxes of objects?
        bool mDrawDebugData;

        // Objects stuff
        ObjectMapTypeAccess mObjects;
        ObjectMapTypeSorted mObjectsSorted;
        uint16_t mNumObjects;

    public:
        LevelManager();

        // Level stuff
        void setForegroundTexture(const std::string &resourceName);
        std::string getForegroundTexture();

        bool getForegroundEnabled();

        bool getForegroundFixed();
        void setForegroundFixed(bool fixed);

        uint16_t getForegroundSize();
        void setForegroundSize(uint16_t size);

        std::string getLevelName();
        void setLevelName(const std::string &name);

        // Camera stuff
        void setCamViewport(const CL_Rectf &viewport);
        CL_Rectf getCamViewport();

        void translateCamera(float x, float y);
        CL_Rectf getAbsoluteCameraPos();

        float getCameraSpeed();
        void setCameraSpeed(float speed);

        void setDrawDebugData(bool draw);

        // Init level manager with level texture
        void init(const std::string &textureName);

        // Objects stuff
        void addObject(const std::string &name, boost::shared_ptr<Object> obj);
        void updateVisual(float elapsed);
        void updateLogic(float elapsed);
};

inline LevelManager &levelManager() { return LevelManager::get_mutable_instance(); }

#endif /* _CORE_LEVEL_MANAGER_HPP_ */
