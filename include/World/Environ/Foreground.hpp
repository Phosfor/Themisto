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

#ifndef _ENVIRON_FOREGROUND_HPP_
#define _ENVIRON_FOREGROUND_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <boost/cstdlib.hpp>

#include "Core/Utils.hpp"

#include "Core/ApplicationManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/LevelManager.hpp"
#include "World/Environ/Object.hpp"

class Foreground : public EnvironObject
{
    private:
        CL_Image mForeground;
        CL_GraphicContext mGC;

    public:
        Foreground();
        void update(float elapsed);
};

#endif /* _ENVIRON_FOREGROUND_HPP_ */
