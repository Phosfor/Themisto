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

#include "Core/StateManager.hpp"

void StateManager::push(boost::python::object newState)
{
    try
    {
        boost::shared_ptr<State> cpp =
            boost::python::extract< boost::shared_ptr<State> >(boost::python::incref(newState.ptr()));

        StateStorage storage(cpp, newState);
        mStates.push_back(storage);
    }
    catch(boost::python::error_already_set &e)
    {
        LOG("Something bad has been happened with script system when adding state...");
        PyErr_Print();
    }
}

void StateManager::update()
{
    if (mStates.empty()) return;
    if (!mActiveState->getCppObject()) this->pop();

    if (mAdvanceState)
    {
        mActiveState->getCppObject()->shutdown();
        LOG_META(cl_format(" Exiting from '%1' game state.", mActiveState->getCppObject()->type()));
        setAdvanceState(false);

        /*TODO: Remove all entities, etc. */

        this->pop();
    }
    else
    {
        mActiveState->getCppObject()->update();
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

StateStorage StateManager::pop()
{
    if (mStates.empty()) LOG_META("There aren't any states now. Maybe, you didn't push one.");

    mActiveState = &mStates.front(); // Store pointer for next state to use
    mStates.pop_front();            // And remove it from list

    LOG_META(cl_format(" Activating '%1' game state.", mActiveState->getCppObject()->type()));
    mActiveState->getCppObject()->init();

    return *mActiveState;
}

StateStorage StateManager::getActiveState()
{
    if (mActiveState->getCppObject() == NULL) LOG_META("There is no active state. Maybe, you didn't push one.");
    return *mActiveState;
}
