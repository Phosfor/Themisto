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

// Overloaded functions in CL_Vec2 class... (There are also static variants of these)
CL_Vec2<int> &(CL_Vec2<int>::*Vec2iNorm)() = &CL_Vec2<int>::normalize;
int (CL_Vec2<int>::*Vec2iDot)(const CL_Vec2<int>&) const = &CL_Vec2<int>::dot;
CL_Vec2<int> &(CL_Vec2<int>::*Vec2iRound)() = &CL_Vec2<int>::round;

BOOST_PYTHON_MODULE(TypesConverters)
{
    using namespace ScriptTypesConverters;

    ////////////////////////////// TYPES CONVERTERS /////////////////////////////////////
    // std::vector
    bp::to_python_converter<std::vector<std::string>, VecToList<std::string> >();
    bp::to_python_converter< std::vector<int>, VecToList<int> >();
    bp::to_python_converter< std::vector<float>, VecToList<float> >();

    // std::pair
    bp::to_python_converter< std::pair<int, int>, PairToTuple<int, int> >();
    bp::to_python_converter< std::pair<std::string, std::string>, PairToTuple<std::string, std::string> >();


    ////////////////////////////// EXPORTING CLASSES ///////////////////////////////////

    // CL_Point [integer] -------------------------------------------------------------
    bp::class_<CL_Point>("CL_Point", bp::init<int, int>())
        .def(bp::init< CL_Vec2<int> >());

    // CL_Point [float] -------------------------------------------------------------
    bp::class_<CL_Pointf>("CL_Pointf", bp::init<float, float>())
    .def(bp::init< CL_Vec2<float> >());

    // CL_Angle ----------------------------------------------------------------------
    bp::class_<CL_Angle>("CL_Angle")
        .def("FromRadians", CL_Angle::from_radians).staticmethod("FromRadians")
        .def("FromDegrees", CL_Angle::from_degrees).staticmethod("FromDegrees")
        .def("Normalize", &CL_Angle::normalize, bp::return_value_policy<bp::copy_non_const_reference>())
        .def("ToDegrees", &CL_Angle::to_degrees)
        .def("ToRadians", &CL_Angle::to_radians)

        // Export overriden operators
        .def(bp::self += CL_Angle()).def(bp::self -= CL_Angle())
        .def(bp::self + CL_Angle()) .def(bp::self - CL_Angle())
        .def(bp::self *= CL_Angle()).def(bp::self /= CL_Angle())
        .def(bp::self * CL_Angle()) .def(bp::self / CL_Angle())
        .def(bp::self == CL_Angle()).def(bp::self != CL_Angle())
        .def(bp::self < CL_Angle()) .def(bp::self > CL_Angle())
        .def(bp::self <= CL_Angle()) .def(bp::self >= CL_Angle());

    // CL_Vec2 [integer] --------------------------------------------------------------
    bp::class_< CL_Vec2<int> >("CL_Vec2i", bp::init<int, int>())
        .def("Length", &CL_Vec2<int>::length)
        .def("Dot", Vec2iDot)
        .def("Normalize", Vec2iNorm, bp::return_value_policy<bp::copy_non_const_reference>())
        .def("Angle", &CL_Vec2<int>::angle)
        .def("Distance", &CL_Vec2<int>::distance)
        .def("Round", Vec2iRound, bp::return_value_policy<bp::copy_non_const_reference>())
        .def(bp::self_ns::str(bp::self_ns::self))  // Export __str__

        // Export overriden operators
        .def(bp::self += CL_Vec2<int>()).def(bp::self -= CL_Vec2<int>())
        .def(bp::self + CL_Vec2<int>()) .def(bp::self - CL_Vec2<int>())
        .def(bp::self *= CL_Vec2<int>()).def(bp::self /= CL_Vec2<int>())
        .def(bp::self * CL_Vec2<int>()) .def(bp::self / CL_Vec2<int>())
        .def(bp::self == CL_Vec2<int>()).def(bp::self != CL_Vec2<int>())
        .def(bp::self < CL_Vec2<int>()) .def(bp::self > CL_Vec2<int>())
        .def(bp::self <= CL_Vec2<int>()) .def(bp::self >= CL_Vec2<int>());

    // CL_Size [integer] --------------------------------------------------------------
    bp::class_<CL_Size>("CL_Size", bp::init<int, int>())
        .def(bp::init< CL_Sizex<int> >())
        .def(bp::init< CL_Sizex<float> >())

        .def_readwrite("width", &CL_Size::width)
        .def_readwrite("height", &CL_Size::height)

        .def(bp::self_ns::str(bp::self_ns::self))  // Export __str__

        // Export overriden operators
        .def(bp::self += int()).def(bp::self -= int())
        .def(bp::self + int()) .def(bp::self - int())
        .def(bp::self *= int()).def(bp::self /= int())
        .def(bp::self * int()) .def(bp::self / int())
        .def(bp::self == CL_Size()).def(bp::self != CL_Size());

    // CL_Size [float] -----------------------------------------------------------------
    bp::class_<CL_Sizef>("CL_Sizef", bp::init<float, float>())
        .def(bp::init< CL_Sizex<int> >())
        .def(bp::init< CL_Sizex<float> >())

        .def_readwrite("width", &CL_Sizef::width)
        .def_readwrite("height", &CL_Sizef::height)

        .def(bp::self_ns::str(bp::self_ns::self))  // Export __str__

        // Export overriden operators
        .def(bp::self += float()).def(bp::self -= float())
        .def(bp::self + float()) .def(bp::self - float())
        .def(bp::self *= float()).def(bp::self /= float())
        .def(bp::self * float()) .def(bp::self / float())
        .def(bp::self == CL_Sizef()).def(bp::self != CL_Sizef());

    // CL_Rect [integer] ---------------------------------------------------------------
    bp::class_<CL_Rect>("CL_Rect", bp::init<int, int, int, int>())
        .def(bp::init< CL_Rectx<int> >())
        .def(bp::init< CL_Rectx<float> >())
        .def(bp::init< CL_Sizex<int> >())

        .def_readonly("left", &CL_Rect::left)
        .def_readonly("top", &CL_Rect::top)
        .def_readonly("right", &CL_Rect::right)
        .def_readonly("bottom", &CL_Rect::bottom)

        .def(bp::self_ns::str(bp::self_ns::self));  // Export __str__

    bp::class_<CL_DisplayWindow>("CL_DisplayWindow")
        .def("GetMinimumSize", &CL_DisplayWindow::get_minimum_size, GetMinimumSizeOverloads());
}
