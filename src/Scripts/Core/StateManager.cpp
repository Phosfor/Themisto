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

#include "Core/ScriptsManager.hpp"
#include "Core/StateManager.hpp"

struct StateWrap : State, bp::wrapper<State>
{
    void init()
    {
        this->get_override("Init")();
    }
    void update()
    {
        this->get_override("Update")();
    }
    void shutdown()
    {
        this->get_override("Shutdown")();
    }
    std::string type()
    {
        return this->get_override("Type")();
    }
};

BOOST_PYTHON_MODULE(StateManager)
{
    bp::class_<StateWrap, boost::noncopyable>("State")
        .def("Init", bp::pure_virtual(&State::init))
        .def("Update", bp::pure_virtual(&State::update))
        .def("Shutdown", bp::pure_virtual(&State::shutdown))
        .def("Type", bp::pure_virtual(&State::type));

    bp::class_<StateManager, boost::noncopyable>("StateManager", bp::no_init)
        .def("Push", &StateManager::push)
        .def("Update", &StateManager::update)
        .def("SetAdvanceState", &StateManager::setAdvanceState)

        //.def("GetActiveState", &StateManager::getActiveState)
        .def("Pop", &StateManager::pop);

    bp::def("getInstance", &stateManager, PYPOLICY_REFERENCE_EXISTING);
    bp::def("ReadyToAdvance", &READY_TO_ADVANCE);
}
