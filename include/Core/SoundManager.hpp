#ifndef _SOUND_MANAGER_HPP_
#define _SOUND_MANAGER_HPP_

#include <ClanLib/display.h>
#include <ClanLib/core.h>

#define soundManager (SoundManager::get_mutable_instance())
#define soundManagerConst (SoundManager::get_const_instance())

class SoundManager : public boost::serialization::singleton<SoundManager>
{
    private:
        CL_SoundOutput mOutput;
};

#endif /* _SOUND_MANAGER_HPP_ */
