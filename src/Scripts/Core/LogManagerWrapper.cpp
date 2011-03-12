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

#include <boost/python.hpp>
//#include "Core/LogManager.hpp"

using namespace boost::python;
struct World
{
    World(std::string msg): msg(msg) {} // added constructor
    void set(std::string msg) { this->msg = msg; }
    std::string greet() { return msg; }
    std::string msg;
};

BOOST_PYTHON_MODULE(script)
{
    class_<World>("World", init<std::string>())
        .def("greet", &World::greet)
        .def("set", &World::set)
    ;
    /*class_< LogManager, boost::serialization::singleton<LogManager> >("LogManager", no_init)
        .def("setDefaultLog", &LogManager::setDefaultLog)
    ;*/
}
