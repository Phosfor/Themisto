/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
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
