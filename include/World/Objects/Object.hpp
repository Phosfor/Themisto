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

#ifndef _WORLD_OBJECT_HPP_
#define _WORLD_OBJECT_HPP_

#include <ClanLib/core.h>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

class Action;
class Object : public boost::noncopyable
{
    protected:
        std::string mName;
        std::string mType;
        int z_index;
        bool mAlwaysDraw;

    public:
        virtual std::string getName() { return mName; }
        virtual void setName(const std::string &name) { mName = name; }

        virtual int getIndex() { return z_index; }
        virtual void setIndex(int _z_index) { z_index = _z_index; }

        virtual bool getAlwaysDraw() { return mAlwaysDraw; }
        virtual void setAlwaysDraw(bool state) { mAlwaysDraw = state; }

        virtual std::string getType() { return mType; }
        virtual void setType(const std::string &typeName) { mType = typeName; }

        virtual void setPosition(CL_Pointf newPos) = 0;
        virtual CL_Pointf getPosition() = 0;
        virtual CL_Rectf getRectangle() = 0;

        virtual bool checkCollision(CL_Pointf point) = 0;

        virtual void init() = 0;
        virtual void update(float elapsed) = 0;
        virtual void updateVisual(float newX, float newY) = 0;

        virtual std::vector< boost::shared_ptr<Action> > getAvailableActions()
        { return std::vector< boost::shared_ptr<Action> >();}
};

#endif /* _WORLD_OBJECT_HPP_ */
