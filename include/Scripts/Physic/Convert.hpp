#pragma once
#include <boost/python.hpp>

template <typename T>
boost::python::object convert_ptr(T *ptr)
{
    typename boost::python::reference_existing_object::apply<T *>::type converter;
    return boost::python::object(boost::python::handle<>(converter(ptr)));
}
