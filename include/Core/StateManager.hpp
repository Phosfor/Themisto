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

#ifndef _STATE_MANAGER_HPP_
#define _STATE_MANAGER_HPP_

#include <boost/serialization/singleton.hpp>
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "Core/LogManager.hpp"
#include "Core/Utils.hpp"

#include <deque>
#include <ClanLib/core.h>

class State
{
    public:
        virtual ~State() { }
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;
        virtual std::string type() = 0;
};

// TODO: Replace with shared_array from boost
typedef boost::shared_ptr<State> StatePtr;
typedef std::deque<StatePtr> StateDeque;

class StateManager : public boost::serialization::singleton<StateManager>
{
   private:
        StateDeque mStates;
        StatePtr mActiveState;

        bool mAdvanceState;

    public:
        StateManager();
        //~StateManager();

        void push(State *state);
        void update();
        void setAdvanceState(bool advance);

        State *getActiveState();
        State *pop();
};

inline StateManager &stateManager() { return StateManager::get_mutable_instance(); }
inline void READY_TO_ADVANCE() { StateManager::get_mutable_instance().setAdvanceState(true); }

#endif /* _STATE_MANAGER_HPP_ */
