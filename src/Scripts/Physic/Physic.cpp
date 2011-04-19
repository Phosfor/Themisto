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

#include <Box2D/Box2D.h>

#include "Box2D/Common/b2Math.h"
#include "Box2D/Collision/b2Collision.h"
#include "Box2D/Dynamics/b2World.h"
#include "Box2D/Dynamics/b2Body.h"

#include "Core/ScriptsManager.hpp"
#include "Physic/Body.hpp"

// Wrappers
#include "Scripts/Physic/b2Vertex.hpp"
#include "Scripts/Physic/Convert.hpp"
#include "Scripts/Physic/Operator.hpp"
#include "Scripts/Physic/SharedArrayRange.hpp"
#include "Scripts/Physic/b2Body.hpp"
#include "Scripts/Physic/b2Vec2.hpp"
#include "Scripts/Physic/b2Joint.hpp"
#include "Scripts/Physic/b2Shape.hpp"
#include "Scripts/Physic/b2Fixture.hpp"
#include "Scripts/Physic/b2World.hpp"

boost::shared_ptr<Body> ParsePhysicBody(CL_DomElement body);
BOOST_PYTHON_MODULE(Physic)
{
    // Our wrapper
    bp::class_<Body, boost::noncopyable>("Body")
        .def("GetBody", &Body::getBody, PYPOLICY_REFERENCE_EXISTING)
        .def("SetBody", &Body::setBody);

    // DOM parser
    bp::def("ParsePhysicBody", &ParsePhysicBody);

    bp::class_<b2AABB>("b2AABB")
        .add_property("LowerBound", &b2AABB::lowerBound)
        .add_property("UpperBound", &b2AABB::upperBound)
        .def("Combine", &b2AABB::Combine);

    wrap_body_type();
    wrap_joint_type();
    wrap_mass_data();
    wrap_shape_type();
    wrap_vec_2();
    wrap_vertex_array();

    wrap_shape();
    wrap_circle_shape();
    wrap_polygon_shape();

    wrap_world();
    wrap_body();
    wrap_fixture();

    wrap_joint();
    wrap_revolute_joint();
    wrap_prismatic_joint();
    wrap_distance_joint();
    wrap_pulley_joint();
    wrap_mouse_joint();
    wrap_gear_joint();
    wrap_line_joint();
    wrap_weld_joint();
    wrap_friction_joint();
}
