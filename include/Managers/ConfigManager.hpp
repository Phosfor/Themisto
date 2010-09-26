#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/foreach.hpp>

#include <vector>

using namespace boost::serialization;
using boost::property_tree::ptree;
using namespace std;

#define configManager (ConfigManager::get_mutable_instance())
#define configManagerConst (ConfigManager::get_const_instance())

#define CONFIG(key, type, def) configManager.getValue<type>(key, def)

class ConfigManager : public singleton<ConfigManager>
{
    protected:
        string mConfigPath;
        ptree mTreeHandle;

    public:
        ConfigManager();
        void setPath(const string &path);
        vector<string> getListValue(const string &key);

        template<typename T>
        T getValue(const string &key, const T &defaultValue = T())
        {
            return mTreeHandle.get<T>(key, defaultValue);
        }
};

#endif
