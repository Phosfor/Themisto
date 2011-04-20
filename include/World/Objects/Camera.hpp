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

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>

#include "Core/ConfigManager.hpp"
#include "Core/Utils.hpp"

class Camera
{
    private:
        float mCameraSpeed;
        CL_Rectf mCameraViewport;

        bool mDrawDebugData, mDrawDebugOnly;

    public:
        Camera();

        void setCamViewport(const CL_Rectf &viewport);
        CL_Rectf getCamViewport() const;

        void translateCamera(float x, float y);
        CL_Rectf getAbsoluteCameraPos() const;

        float getCameraSpeed() const;
        void setCameraSpeed(float speed);

        bool getDrawDebugData() const;
        void setDrawDebugData(bool draw);

        bool getDrawDebugOnly() const;
        void setDrawDebugOnly(bool draw);
};
