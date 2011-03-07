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

#include "World/Actions/TestAction.hpp"



bool TestAction::executing()
{
    return mExecuting;
}

std::string TestAction::getName()
{
    return "Test";
}
std::string TestAction::getDescription()
{
    return "Тестовое действие, которое ничего не делает";
}
CL_Sprite TestAction::getIcon()
{
    return resourceManager().getSprite("Actions", "take");
}

void TestAction::beginExecution(Actor* actor)
{
    std::cout<< "Test action is begining execution" << std::endl;
    mTadam = 0;
    mExecuting = true;
}
void TestAction::stopExecution()
{
    std::cout<<"Test action is stoping execution" << std::endl;
    mExecuting = false;
}
bool TestAction::canExecute(Actor* actor)
{
    std::cout << "Yes, test action is always ready to execute : )" << std::endl;
    return true;
}
void TestAction::update(float step)
{
    mTadam++;
    std::cout << "Test action on " << mTadam << " iteration "<< std::endl;
    if(mTadam > 10000) stopExecution();
}
void TestAction::updateVisual(float actorX, float actorY)
{
}
