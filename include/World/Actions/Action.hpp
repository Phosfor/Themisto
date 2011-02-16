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

#ifndef _ACTION_HPP_
#define _ACTION_HPP_

#include <ClanLib/display.h>
#include "Core/Utils.hpp"

class Actor;

class Action
{
    public:
        CL_Signal_v1<Action*> mExecutionBegun;
        CL_Signal_v1<Action*> mExecutionCompleted;
        CL_Signal_v1<Action*> mExecutionStopped;

        virtual std::string getName() = 0;
        virtual std::string getDescription() = 0;
        virtual CL_Sprite getIcon() = 0;
        virtual void beginExecution(Actor* actor) = 0;
        virtual void stopExecution() = 0;
        virtual bool canExecute(Actor* actor) = 0;
        virtual bool executing() = 0;
        virtual void update(float step) = 0;
        virtual void updateVisual(float actorX, float actorY) = 0;
};

#endif
