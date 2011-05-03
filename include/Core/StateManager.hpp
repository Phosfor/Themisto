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

#include <deque>

#include <boost/serialization/singleton.hpp>
#include <boost/foreach.hpp>
#include <boost/python.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <ClanLib/core.h>

#include "Core/LogManager.hpp"
#include "Core/Utils.hpp"

class State
{
    public:
        virtual ~State() { }
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;
        virtual std::string type() = 0;
};

class StateStorage
{
    private:
        boost::shared_ptr<State> mCppObject;
        boost::python::object mPythonObject;

    public:
        StateStorage(boost::shared_ptr<State> cpp, boost::python::object python)
            : mCppObject(cpp), mPythonObject(python) {}

        boost::shared_ptr<State> getCppObject() const { return mCppObject; }
        boost::python::object getPythonObject() const { return mPythonObject; }
};

typedef std::deque<StateStorage> StateDeque;

class StateManager : public boost::serialization::singleton<StateManager>
{
   private:
        StateDeque mStates;
        StateStorage *mActiveState;

        bool mAdvanceState;

    public:
        StateManager();
        //~StateManager();

        void push(boost::python::object newState);
        void update();
        void setAdvanceState(bool advance);

        StateStorage getActiveState();
        StateStorage pop();
};

inline StateManager &stateManager() { return StateManager::get_mutable_instance(); }
inline void READY_TO_ADVANCE() { StateManager::get_mutable_instance().setAdvanceState(true); }
