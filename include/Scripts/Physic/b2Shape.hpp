#pragma once

boost::shared_ptr<b2CircleShape> construct_circle_shape(b2Vec2 position, float32 radius);
boost::python::list get_polygon_shape_vertices(b2PolygonShape *polygon_shape);
void set_polygon_shape_vertices(b2PolygonShape *polygon_shape, boost::python::list vertices);
boost::shared_ptr<b2PolygonShape> construct_polygon_shape(boost::python::list vertices);
void wrap_mass_data();
void wrap_shape_type();
void wrap_shape();
void wrap_circle_shape();
void wrap_polygon_shape();
