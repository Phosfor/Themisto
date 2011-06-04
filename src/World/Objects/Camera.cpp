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

#include "World/Objects/Camera.hpp"

Camera::Camera(CL_Size levelSize) :
    mTextureSize(levelSize)
{
    mCameraSpeed = configManager().getValue<float>("application.camera_speed", 10.0f);
    mDrawDebugOnly = configManager().getValue<bool>("application.debug_draw_only", false);
    mDrawDebugData = false;
}

void Camera::setViewport(const CL_Rectf  &viewport)
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

CL_Rectf Camera::getViewport() const
{
    return mCameraViewport;
}

void Camera::setDrawDebugData(bool draw)
{
    mDrawDebugData = draw;
}

bool Camera::getDrawDebugData() const
{
    return mDrawDebugData;
}

void Camera::setDrawDebugOnly(bool draw)
{
    mDrawDebugOnly = draw;
}

bool Camera::getDrawDebugOnly() const
{
    return mDrawDebugOnly;
}

float Camera::getSpeed() const
{
    return mCameraSpeed;
}

void Camera::setSpeed(float speed)
{
    mCameraSpeed = speed;
}

void Camera::translate(float x, float y)
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

    mCameraMovedSignal.invoke(x);
    mCameraViewport.translate(x, y);
}

CL_Rectf Camera::getAbsolutePos() const
{
    return CL_Rectf(abs(mCameraViewport.left),
                    abs(mCameraViewport.top),
                    abs(mCameraViewport.left) + ScreenResolutionX,
                    abs(mCameraViewport.top) + ScreenResolutionY);
}

void Camera::pushSlotCameraMoved(CL_Slot camMoved)
{
    mSlotsCameraMoved.push_back(camMoved);
}

CL_Signal_v1<int> &Camera::getSignalCameraMoved()
{
    return mCameraMovedSignal;
}
