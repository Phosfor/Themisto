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

#ifndef _ENVIRON_TYPES_HPP_
#define _ENVIRON_TYPES_HPP_

// Numbers show the priority of render
enum EnvironTypes
{
    Environ_Sky    = 0,
    Environ_Stars  = 1,
    Environ_Moon   = 2,
    Environ_Lightnings = 3,
    Environ_Clouds = 4,
    Environ_Foreground = 5,
    Environ_Rain   = 6,
    Environ_Leaves = 7,
    Environ_Birds  = 8
};

#endif /* _ENVIRON_TYPES_HPP_ */
