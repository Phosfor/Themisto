/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Core/ConfigManager.hpp"

ConfigManager::ConfigManager()
{
    setPath("settings.info");
}

void ConfigManager::setPath(const string &path)
{
    mConfigPath = path;
    read_info(path, mTreeHandle);
}

vector<string> ConfigManager::getListValue(const string &key)
{
    vector<string> listValues;
    BOOST_FOREACH(ptree::value_type &value, mTreeHandle.get_child(key))
    {
        listValues.push_back(value.second.data());
    }

    return listValues;
}
