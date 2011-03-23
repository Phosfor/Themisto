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
#include "World/Objects/Object.hpp"

struct ObjectWrap : Object, bp::wrapper<Object>
{
    // VIRTUAL FUNCTIONS --------------------------------------------------------

    // getName wrapper
    std::string getName()
    {
        if (bp::override getName = this->get_override("getName")) return getName();
        return Object::getName();
    }
    std::string default_getName() { return this->Object::getName(); }

    // setName wrapper
    void setName(const std::string &name)
    {
        if (bp::override setName = this->get_override("setName")) setName(name);
        Object::setName(name);
    }
    void default_setName(const std::string &name) { this->Object::setName(name); }

    // getIndex wrapper
    int getIndex()
    {
        if (bp::override getIndex = this->get_override("getIndex")) return getIndex();
        return Object::getIndex();
    }
    int default_getIndex() { return this->Object::getIndex(); }

    // setIndex wrapper
    void setIndex(int zIndex)
    {
        if (bp::override setIndex = this->get_override("setIndex")) setIndex(zIndex);
        return Object::setIndex(zIndex);
    }
    void default_setIndex(int zIndex) { return this->Object::setIndex(zIndex); }

    // getAlwaysDraw wrapper
    bool getAlwaysDraw()
    {
        if (bp::override getAlwaysDraw = this->get_override("getAlwaysDraw")) return getAlwaysDraw();
        return Object::getAlwaysDraw();
    }
    bool default_getAlwaysDraw() { return this->Object::getAlwaysDraw(); }

    // setAlwaysDraw wrapper
    void setAlwaysDraw(bool alwaysDraw)
    {
        if (bp::override setAlwaysDraw = this->get_override("setAlwaysDraw")) setAlwaysDraw(alwaysDraw);
        return Object::setAlwaysDraw(alwaysDraw);
    }
    void default_setAlwaysDraw(bool alwaysDraw) { return this->Object::setAlwaysDraw(alwaysDraw); }

    // getType wrapper
    std::string getType()
    {
        if (bp::override getType = this->get_override("getType")) return getType();
        return Object::getType();
    }
    std::string default_getType() { return this->Object::getType(); }

    // setType wrapper
    void setType(const std::string &type)
    {
        if (bp::override setType = this->get_override("setType")) setType(type);
        return Object::setType(type);
    }
    void default_setType(const std::string &type) { return this->Object::setType(type); }


    // getAvailableActions wrapper
    std::vector< boost::shared_ptr<Action> > getAvailableActions()
    {
        if (bp::override getType = this->get_override("getAvailableActions")) return getType();
        return Object::getAvailableActions();
    }
    std::vector< boost::shared_ptr<Action> > default_getAvailableActions() { return this->Object::getAvailableActions(); }

    // PURE VIRTUAL FUNCTIONS ------------------------------------------------------
    void setPosition(CL_Pointf newPos)
    {
        this->get_override("setPosition")(newPos);
    }

    CL_Pointf getPosition()
    {
        return this->get_override("getPosition")();
    }

    CL_Rectf getRectangle()
    {
        return this->get_override("getRectangle")();
    }

    bool checkCollision(CL_Pointf point)
    {
        return this->get_override("checkCollision")(point);
    }

    void init()
    {
        this->get_override("init")();
    }

    void update(float elapsed)
    {
        this->get_override("update")(elapsed);
    }

    void updateVisual(float newX, float newY)
    {
        this->get_override("updateVisual")(newX, newY);
    }
};

BOOST_PYTHON_MODULE(Object)
{
    bp::class_<ObjectWrap, boost::noncopyable>("Object", bp::no_init)
        .def("GetName", &Object::getName, &ObjectWrap::default_getName)
        .def("SetName", &Object::setName, &ObjectWrap::default_setName)

        .def("GetIndex", &Object::getIndex, &ObjectWrap::default_getIndex)
        .def("SetIndex", &Object::setIndex, &ObjectWrap::default_setIndex)

        .def("GetAlwaysDraw", &Object::getAlwaysDraw, &ObjectWrap::default_getAlwaysDraw)
        .def("SetAlwaysDraw", &Object::setAlwaysDraw, &ObjectWrap::default_setAlwaysDraw)

        .def("GetType", &Object::getType, &ObjectWrap::default_getType)
        .def("SetType", &Object::setType, &ObjectWrap::default_setType)

        .def("SetPosition", bp::pure_virtual(&Object::setPosition))
        .def("GetPosition", bp::pure_virtual(&Object::getPosition))

        .def("GetRectangle", bp::pure_virtual(&Object::getRectangle))

        .def("CheckCollision", bp::pure_virtual(&Object::checkCollision))

        .def("Init", bp::pure_virtual(&Object::init))
        .def("Update", bp::pure_virtual(&Object::update))
        .def("UpdateVisual", bp::pure_virtual(&Object::updateVisual))

        .def("GetAvailableActions", &Object::getAvailableActions, &ObjectWrap::default_getAvailableActions);
}
