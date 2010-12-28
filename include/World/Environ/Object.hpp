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

#ifndef _ENVIRON_OBJECT_HPP_
#define _ENVIRON_OBJECT_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class EnvironObject
{
   protected:
      int mMaxObjects;
      int mWindowWidth, mWindowHeight;
      bool mEnabled;

   public:
      EnvironObject();

      virtual void setLimit(int limit);
      virtual int getLimit();

      bool getEnabled();
      void setEnabled(bool state);

      virtual void update(float windPower, float elapsed, float globalTime) = 0;
};

#endif
