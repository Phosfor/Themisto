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

#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <vector>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/singleton.hpp>

class ConfigManager : public boost::serialization::singleton<ConfigManager>
{
    protected:
        std::string mConfigPath;
        boost::property_tree::ptree mTreeHandle;

    public:
        ConfigManager();
        void setPath(const std::string &path);
        std::vector<std::string> getListValue(const std::string &key);

        template<typename T>
        T getValue(const std::string &key, const T &defaultValue = T())
        {
            return mTreeHandle.get<T>(key, defaultValue);
        }
};

inline ConfigManager &configManager() { return ConfigManager::get_mutable_instance(); }

#endif /* _CONFIG_MANAGER_H_ */
