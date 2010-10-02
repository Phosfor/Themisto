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
