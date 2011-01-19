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

#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

#include <vector>

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/gui.h>

#include <ClanLib/gl.h>
#include <ClanLib/gl1.h>
#include <ClanLib/swrender.h>

#include <ClanLib/sound.h>
#include <ClanLib/mikmod.h>
#include <ClanLib/vorbis.h>

#include "Core/LogManager.hpp"
#include "Core/ConfigManager.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/StateManager.hpp"
#include "Core/GuiManager.hpp"
#include "Core/Utils.hpp"
#include "Core/InputManager.hpp"
#include "World/Objects/TypesManager.hpp"

#include "World/States/MenuState.hpp"
#include "World/States/EditorState.hpp"

// This is wrapper to make the Main function crossplatform
class Application
{
    public:
        static int main(const std::vector<CL_String> &args);

    private:
        // Slots
        static void onInput(const CL_InputEvent &key, const CL_InputState &state);
        static void onWindowClose();
        static void doNothing();
};

#endif /* _APPLICATION_HPP_ */
