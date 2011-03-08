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

#include "World/Objects/Empty.hpp"

EmptyObject::EmptyObject()
{
    this->setType("Empty");
}
void EmptyObject::updateVisual(float newX, float newY)
{
    LOG("You shoudln't update empty object!");
}

void EmptyObject::update(float elapsed)
{
    LOG("You shoudln't update empty object!");
}

CL_Pointf EmptyObject::getPosition() { return CL_Pointf(); }
void EmptyObject::setPosition(CL_Pointf newPos) { }
CL_Rectf EmptyObject::getRectangle() { return CL_Rectf(); }
bool EmptyObject::checkCollision(CL_Pointf point) { return false; }
