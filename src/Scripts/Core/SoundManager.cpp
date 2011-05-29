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
#include "Core/SoundManager.hpp"

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(PlaySoundBufferOverloads, CL_SoundBuffer::play, 0, 1);

BOOST_PYTHON_MODULE(SoundManager)
{
    bp::class_<CL_SoundBuffer>("CL_SoundBuffer")
        .def("SetVolume", &CL_SoundBuffer::set_volume)
        .def("GetVolume", &CL_SoundBuffer::get_volume)
        .def("Play", &CL_SoundBuffer::play, PlaySoundBufferOverloads());

    bp::class_<CL_SoundBuffer_Session>("CL_SoundBuffer_Session");

    bp::class_<SoundObject>("SoundObject")
        .def("GetBuffer", &SoundObject::getBuffer, PYPOLICY_REFERENCE_EXISTING)
        .def("GetSession", &SoundObject::getSession, PYPOLICY_REFERENCE_EXISTING);

    bp::class_<SoundManager, boost::noncopyable>("SoundManager", bp::no_init)
        .def("GetSoundObject", &SoundManager::getSoundObject, PYPOLICY_REFERENCE_EXISTING)
        .def("AddSoundObject", &SoundManager::addSoundObject, PYPOLICY_REFERENCE_EXISTING);

    bp::def("getInstance", &soundManager, PYPOLICY_REFERENCE_EXISTING);
}
