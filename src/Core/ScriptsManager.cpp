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

ScriptsManager::~ScriptsManager()
{
    Py_Finalize();
}

ScriptsManager::ScriptsManager()
{
    try
    {
        Py_Initialize();
        mMainModule = bp::import("__main__");
        mMainNamespace = mMainModule.attr("__dict__");

        processPaths();
        registerTypesConverters();
    }
    catch(...)
    {
        LOG("Catched some exception at scripts initialization!");
        PyErr_Print();
    }
}

void ScriptsManager::processPaths()
{
    namespace bf = boost::filesystem;
    if (!bf::exists("lib")) LOG("`lib` directory doesn't exists! Rebuild project.");

    // Process python paths
    runString("import sys");
    runString("sys.path.append('lib')");

    std::ofstream init("lib/__init__.py");
    init.close();

    // We need this for sexy python modules
    bf::directory_iterator endIt;
    for ( boost::filesystem::recursive_directory_iterator end, dir("lib/"); 
           dir != end; ++dir )
    {
        if (bf::is_directory(dir->path()))
        {
            if (!bf::exists(cl_format("%1/__init__.py", dir->path().c_str()).c_str()))
            {
                std::ofstream init(cl_format("%1/__init__.py", dir->path().c_str()).c_str());
                init.close();
            }
        }
    }
}

void ScriptsManager::runString(const std::string &pyCode)
{
    try
    {
        // Run command
        bp::exec(pyCode.c_str(), mMainNamespace);
    }
    catch (...)
    {
        LOG(cl_format("Failed to run python code:\n------------------\n%1\n------------------", pyCode));
        PyErr_Print();
    }
}

void ScriptsManager::runFile(const std::string &fileName)
{
    try
    {
        bp::exec_file(fileName.c_str(), mMainNamespace);
    }
    catch (...)
    {
        LOG(cl_format("Failed to run python file script: %1", fileName));
        PyErr_Print();
    }
}

bp::object &ScriptsManager::getMainModule()
{
    return mMainModule;
}

bp::object &ScriptsManager::getMainNamespace()
{
    return mMainNamespace;
}

void ScriptsManager::registerTypesConverters()
{
    runString("from Core.TypesConverters import *");
}

bp::object getAttr(const bp::object &obj, const std::string &attrName)
{
    try
    {
        return obj.attr(boost::python::str(attrName));

    }
    catch(const bp::error_already_set &err)
    {
        /* We need to fetch the error indicators *before*
         * importing anything, as apparently importing
         * using boost python clears the error flags.
         */

        PyObject *e, *v, *t;
        PyErr_Fetch(&e, &v, &t);

        boost::python::object AttributeError = boost::python::import("exceptions").attr("AttributeError");

        /* Squash the exception only if it's an AttributeError, otherwise
         * let the exception propagate.
         */
        if (PyErr_GivenExceptionMatches(AttributeError.ptr(), e))
            return boost::python::object(); // None

        else
            throw;
    }
}
