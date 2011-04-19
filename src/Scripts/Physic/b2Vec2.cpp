#include <boost/python.hpp>
#include <Box2D/Common/b2Math.h>

#include "Scripts/Physic/b2Vec2.hpp"

struct Vec2FromTuple
{
    Vec2FromTuple()
    {
        boost::python::converter::registry::push_back(&convertible, &construct, boost::python::type_id<b2Vec2>());
    }

    static void *convertible(PyObject *obj_ptr)
    {
        return PyTuple_Check(obj_ptr) && PyTuple_Size(obj_ptr) == 2 ? obj_ptr : 0;
    }

    static void construct(PyObject *obj_ptr, boost::python::converter::rvalue_from_python_stage1_data *data)
    {
        double x = PyFloat_AsDouble(PyTuple_GetItem(obj_ptr, 0));
        double y = PyFloat_AsDouble(PyTuple_GetItem(obj_ptr, 1));
        void *storage = ((boost::python::converter::rvalue_from_python_storage<b2Vec2> *) data)->storage.bytes;
        new (storage) b2Vec2(x, y);
        data->convertible = storage;
    }
};

boost::shared_ptr<b2Vec2> construct()
{
    return boost::shared_ptr<b2Vec2>(new b2Vec2(0.0f, 0.0f));
}

int32 len(b2Vec2 *v)
{
    return 2;
}

float32 getitem(b2Vec2 *v, int32 index)
{
    if (index >= 0 && index < 2)
        return (*v)(index);
    else
        throw std::out_of_range("Vec2 index out of range");
}

void setitem(b2Vec2 *v, int32 index, float32 value)
{
    if (index >= 0 && index < 2)
        (*v)(index) = value;
    else
        throw std::out_of_range("Vec2 index out of range");
}

std::string repr(const b2Vec2 &v)
{
    std::ostringstream out;
    out << "Vec2(" << v.x << ", " << v.y << ")";
    return out.str();
}

void wrap_vec_2()
{
    Vec2FromTuple();

    boost::python::class_<b2Vec2>("b2Vec2", boost::python::init<float32, float32>())
        .def("__init__", boost::python::make_constructor(&construct))

        .def(boost::python::self += boost::python::self)
        .def(boost::python::self -= boost::python::self)
        .def(boost::python::self *= float32())

        .def(-boost::python::self)
        .def(boost::python::self + boost::python::self)
        .def(boost::python::self - boost::python::self)
        .def(float32() * boost::python::self)
        .def(boost::python::self == boost::python::self)

        .def("__len__", &len)
        .def("__getitem__", &getitem)
        .def("__setitem__", &setitem)

        .def("__abs__", &b2Vec2::Length)
        .def("__repr__", &repr)

        .def_readwrite("x", &b2Vec2::x)
        .def_readwrite("y", &b2Vec2::y)
        .add_property("Valid", &b2Vec2::IsValid)

        .def("Normalize", &b2Vec2::Normalize)
    ;

    float32 (*dot)(const b2Vec2 &, const b2Vec2 &) = &b2Dot;
    float32 (*cross_1)(const b2Vec2 &, const b2Vec2 &) = &b2Cross;
    b2Vec2 (*cross_2)(const b2Vec2 &, float32) = &b2Cross;
    b2Vec2 (*cross_3)(float32, const b2Vec2 &) = &b2Cross;

    boost::python::def("Dot", dot);
    boost::python::def("Cross", cross_1);
    boost::python::def("Cross", cross_2);
    boost::python::def("Cross", cross_3);
}
