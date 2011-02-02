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

#ifndef _BODY_HPP_
#define _BODY_HPP_

#include <list>
#include <map>
#include <boost/shared_ptr.hpp>
#include "World/WorldManager.hpp"
#include "Physic/Impact.hpp"
#include <Box2D/Box2D.h>

class Body
{
    private:
        std::string mName;
        b2Body* mBody;
        std::map<std::string, std::string> mProperties;
        std::list< boost::shared_ptr<Impact> > mAppliedImpacts;

    public:
        Body();
        ~Body();

        CL_Signal_v1< boost::shared_ptr<Impact> > mImpactApplied;
        CL_Signal_v1< boost::shared_ptr<Impact> > mImpactCanceled;
        CL_Signal_v2<std::string, std::string> mPropertyChanged;

        std::string getProperty(const std::string &name);
        void setProperty(const std::string &name, const std::string &value);

        void applyImpact(boost::shared_ptr<Impact> impact);
        void cancelImpact(boost::shared_ptr<Impact> impact);
        std::list< boost::shared_ptr<Impact> > getAppliedImpacts();

        b2Body* getBody();
        void setBody(b2Body* body);

        std::string getName();
        void setName(const std::string &value);

        // Should body free memory under mBody object at destroing
        // Default is false
        bool mShouldFreeB2Body;
};

#endif
