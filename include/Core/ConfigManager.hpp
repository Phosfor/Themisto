/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/singleton.hpp>

#include <vector>

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
