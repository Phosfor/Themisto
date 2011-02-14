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

#include "World/Objects/Actor.hpp"


// Actions
void Actor::executeAction(boost::shared_ptr<Action> action)
{
        mActiveAction = action;
        action->beginExecution(this);
}
void Actor::stopActiveAction()
{
    if(mActiveAction)
    {
        mActiveAction->stopExecution();
        mActiveAction = boost::shared_ptr<Action>();
    }
}
boost::shared_ptr<Action> Actor::getActiveAction()
{
    return mActiveAction;
}
bool Actor::canExecuteAction(boost::shared_ptr<Action> action)
{
    return mActiveAction->canExecute(this);
}
void Actor::update(float elapsed)
{
    if(mActiveAction)
    {
        mActiveAction->update(elapsed);
    }
}
void Actor::updateVisual(float newX, float newY)
{
    if(mActiveAction)
    {
        mActiveAction->updateVisual(newX, newY);
    }
}

