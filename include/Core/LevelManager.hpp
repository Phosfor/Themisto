/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

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
#include <boost/cstdint.hpp>

#include "Core/ApplicationManager.hpp"
#include "Core/Utils.hpp"

class LevelManager : public boost::serialization::singleton<LevelManager>
{
    private:
        CL_Image mLevelTexture;
        std::string mForegroundTexture;

        // Is foreground image enabled
        bool mForeground;
        bool mFixedForeground;
        int mForegroundDelta; // If level texture is < then window height
        // Average real foreground image size refer to full window size (in percents)
        uint16_t mForegroundActualSize;

        CL_Rectf mCameraViewport;
        CL_GraphicContext mGC;

    public:
        void setForegroundTexture(const std::string &resourceName);
        std::string getForegroundTexture();
        bool getForegroundEnabled();
        bool getForegroundFixed();
        uint16_t getForegroundSize();
        void setForegroundSize(uint16_t size);
        void setForegroundFixed(bool fixed);

        void setCamViewport(const CL_Rectf &viewport);
        CL_Rectf getCamViewport();
        void translateCamera(float x, float y);

        void init(const std::string &textureName);
        void update();
};

inline LevelManager &levelManager() { return LevelManager::get_mutable_instance(); }

#endif /* _CORE_LEVEL_MANAGER_HPP_ */
