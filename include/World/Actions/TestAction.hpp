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

#include "World/Actions/Action.hpp"
#include "Core/ResourceManager.hpp"
#include "Physic/Body.hpp"
#include "Core/PhysicManager.hpp"
#include "World/Objects/Actor.hpp"

class TestAction: public Action
{
    private:
        int mTadam;
        bool mExecuting;
    public:

        // Action implementation
        std::string getName();
        std::string getDescription();
        CL_Sprite getIcon();
        std::vector<std::string> getTextureInfo();
        void beginExecution(Actor* actor);
        void stopExecution();
        bool canExecute(Actor* actor);
        bool executing();
        void update(float step);
        void updateVisual(float actorX, float actorY);
};
