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

#pragma once

#include <map>

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/sound.h>

struct SoundObject
{
    private:
        std::string mFileName;
        CL_SoundBuffer mBuffer;
        CL_SoundBuffer_Session mSession;

    public:
        void loadSound(const std::string &fileName)
        {
            mFileName = fileName;
            mBuffer = CL_SoundBuffer(fileName);
            mSession = mBuffer.prepare();
        }

        CL_SoundBuffer &getBuffer()
        {
            return mBuffer;
        }

        CL_SoundBuffer_Session &getSession()
        {
            return mSession;
        }
};

class SoundManager : public boost::serialization::singleton<SoundManager>
{
    private:
        std::map<std::string, SoundObject> mSounds;

    public:
        SoundObject &getSoundObject(const std::string &name)
        {
            if (mSounds.find(name) == mSounds.end()) 
            {
                LOG("There isn't sound in base with name: " + name);
            }
            return mSounds[name];
        }

        SoundObject &addSoundObject(const std::string &name, const std::string &filename)
        {
            if (mSounds.find(name) != mSounds.end())
            {
                LOG("There is already sound in base with name: " + name);
            }

            SoundObject newSound;
            newSound.loadSound(filename);

            mSounds[name] = newSound;
            return mSounds[name];
        }
};

inline SoundManager &soundManager() { return SoundManager::get_mutable_instance(); }
