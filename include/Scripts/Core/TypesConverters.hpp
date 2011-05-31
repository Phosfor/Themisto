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

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/to_python_converter.hpp>

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

    // CL_String8 → Python string --------------------------------------
    struct cl_string8_to_python_str
    {
        static PyObject* convert(CL_String8 const& s)
        {
            return boost::python::incref(boost::python::object(s.c_str()).ptr());
        }
    };

    // Python string → CL_String8 --------------------------------------
    struct cl_string8_from_python_str
    {
        cl_string8_from_python_str()
        {
            bp::converter::registry::push_back(
                &convertible,
                &construct,
                boost::python::type_id<CL_String8>()
            );
        }

        static void* convertible(PyObject* obj_ptr)
        {
            if (!PyString_Check(obj_ptr)) return 0;
            return obj_ptr;
        }

        static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
        {
            const char* value = PyString_AsString(obj_ptr);
            if (value == 0) bp::throw_error_already_set();
            void* storage = ((bp::converter::rvalue_from_python_storage<CL_String8>*)data)->storage.bytes;
            new (storage) CL_String8(value);
            data->convertible = storage;
        }
    };

    // CL_StringRef8 → Python string --------------------------------------
    struct cl_stringref8_to_python_str
    {
        static PyObject* convert(CL_StringRef8 const& s)
        {
            return boost::python::incref(boost::python::object(s.c_str()).ptr());
        }
    };

    // Python string → CL_StringRef8 --------------------------------------
    struct cl_stringref8_from_python_str
    {
        cl_stringref8_from_python_str()
        {
            bp::converter::registry::push_back(
                &convertible,
                &construct,
                boost::python::type_id<CL_StringRef8>()
            );
        }

        static void* convertible(PyObject* obj_ptr)
        {
            if (!PyString_Check(obj_ptr)) return 0;
            return obj_ptr;
        }

        static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
        {
            const char* value = PyString_AsString(obj_ptr);
            if (value == 0) bp::throw_error_already_set();
            void* storage = ((bp::converter::rvalue_from_python_storage<CL_StringRef8>*)data)->storage.bytes;
            new (storage) CL_StringRef8(value);
            data->convertible = storage;
        }
    };

    // Default functions arguments stuff ------------------------------
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GetMinimumSizeOverloads, CL_DisplayWindow::get_minimum_size, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SetAlignmentImageOverloads, CL_Image::set_alignment, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SetAlignmentSpriteOverloads, CL_Sprite::set_alignment, 1, 3);
}
