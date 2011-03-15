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
#include "Core/Utils.hpp"

BOOST_PYTHON_MODULE(Utils)
{
    bp::def("Meters2Pixels", Meters2Pixels);
    bp::def("Pixels2Meters", Pixels2Meters);

    bp::def("LOCATION", LOCATION);
    bp::def("Randf", Randf);

    bp::def("Rad2Deg", Rad2Deg);
    bp::def("Deg2Rad", Deg2Rad);

    bp::def("GameMicroseconds", GameMicroseconds);
    bp::def("GameSeconds", GameSeconds);
    bp::def("GameMinutes", GameMinutes);
    bp::def("GameHours", GameHours);

    bp::scope().attr("Pi") = Pi;
    bp::scope().attr("Gravity") = Gravity;
    bp::scope().attr("TimeKoef") = TimeKoef;
    bp::scope().attr("PixelsPerMeter") = PixelsPerMeter;
    bp::scope().attr("GAME_VERSION") = GAME_VERSION;
    bp::scope().attr("ScreenResolutionX") = ScreenResolutionX;
    bp::scope().attr("ScreenResolutionY") = ScreenResolutionY;
}
