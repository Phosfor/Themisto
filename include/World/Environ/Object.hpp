/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _ENVIRON_OBJECT_HPP_
#define _ENVIRON_OBJECT_HPP_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "Core/Utils.hpp"
#include "Core/ApplicationManager.hpp"

class EnvironObject
{
   protected:
      int mMaxObjects;
      int mWindowWidth, mWindowHeight;
      bool mEnabled;

   public:
      EnvironObject();

      virtual void setLimit(int limit);
      virtual int getLimit();

      bool getEnabled();
      void setEnabled(bool state);

      virtual void update(float windPower, float elapsed, float globalTime) = 0;
};

#endif
