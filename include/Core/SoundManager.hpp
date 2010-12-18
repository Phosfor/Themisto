/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _SOUND_MANAGER_HPP_
#define _SOUND_MANAGER_HPP_

#include <ClanLib/display.h>
#include <ClanLib/core.h>

class SoundManager : public boost::serialization::singleton<SoundManager>
{
    private:
        CL_SoundOutput mOutput;
};

inline SoundManager &soundManager() { return SoundManager::get_mutable_instance(); }

#endif /* _SOUND_MANAGER_HPP_ */
