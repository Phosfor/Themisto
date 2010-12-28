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

#include "Core/StateManager.hpp"

void StateManager::push(State *state)
{
    mStates.push_back(StatePtr(state));
}

void StateManager::update()
{
    if (mActiveState.get() == NULL) this->pop();

    if (mAdvanceState)
    {
        mActiveState.get()->shutdown();
        LOG_META(cl_format(" Exiting from '%1' game state.", mActiveState.get()->type()));
        setAdvanceState(false);

        /*TODO: Remove all entities, etc. */

        this->pop();
    }
    else
    {
        mActiveState.get()->update();
    }
}

StateManager::StateManager(): mAdvanceState(false) { }

/*
StateManager::~StateManager()
{
    // TODO: Delete all entities
    while (!mStates.empty())
    {
        mStates.pop_back();
    }
}
*/

void StateManager::setAdvanceState(bool advance)
{
    mAdvanceState = advance;
}

State *StateManager::pop()
{
    if (mStates.empty()) LOG_META("There aren't any states now. Maybe, you didn't push one.");

    mActiveState = mStates.front(); // Store pointer for next state to use
    mStates.pop_front();            // And remove it from list

    LOG_META(cl_format(" Activating '%1' game state.", mActiveState.get()->type()));
    mActiveState.get()->init();

    return mActiveState.get();
}

State *StateManager::getActiveState()
{
    if (mActiveState.get() == NULL) LOG_META("There is no active state. Maybe, you didn't push one.");
    return mActiveState.get();
}
