#ifndef _IENVIRON_OBJECT_HPP_
#define _IENVIRON_OBJECT_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class EnvironObject
{
   protected:
      int mMaxObjects;
      int mWindowWidth, mWindowHeight;

   public:
      EnvironObject();
      virtual void setLimit(int limit);
      virtual int getLimit();
      virtual void update(float windPower, float elapsed, float globalTime) = 0;
};

#endif
