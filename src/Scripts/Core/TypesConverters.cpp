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
#include "Core/LogManager.hpp"
#include "Core/TypesConverters.hpp"

BOOST_PYTHON_MODULE(TypesConverters)
{
    using namespace ScriptTypesConverters;

    // std::vector
    bp::to_python_converter<std::vector<std::string>, VecToList<std::string> >();
    bp::to_python_converter< std::vector<int>, VecToList<int> >();
    bp::to_python_converter< std::vector<float>, VecToList<float> >();

    // std::pair
    bp::to_python_converter< std::pair<int, int>, PairToTuple<int, int> >();
    bp::to_python_converter< std::pair<std::string, std::string>, PairToTuple<std::string, std::string> >();
}
