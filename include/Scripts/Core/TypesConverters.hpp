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

#include <ClanLib/display.h>
#include <ClanLib/core.h>

#include "Core/ScriptsManager.hpp"
#include "Core/LevelManager.hpp"
#include "Core/LogManager.hpp"

namespace bp = boost::python;
namespace ScriptTypesConverters
{
    // std::vector<T> → Python list -----------------------------------
    template<class T>
    struct VecToList
    {
        static PyObject* convert(const std::vector<T>& vec)
        {
            bp::list* l = new bp::list();
            for(size_t i = 0; i < vec.size(); i++)
                (*l).append(vec[i]);

            return l->ptr();
        }
    };

    // std::pair<T1, T2> → Python tuple --------------------------------
    template<class T1, class T2>
    struct PairToTuple
    {
      static PyObject* convert(const std::pair<T1, T2>& pair) {
        return bp::incref(bp::make_tuple(pair.first, pair.second).ptr());
      }
    };

    // Default functions arguments stuff ------------------------------
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GetMinimumSizeOverloads, CL_DisplayWindow::get_minimum_size, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SetAlignmentImageOverloads, CL_Image::set_alignment, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SetAlignmentSpriteOverloads, CL_Sprite::set_alignment, 1, 3);
}
