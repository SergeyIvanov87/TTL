#ifndef TEST_EF_CUSTOM_CONFIGURATOR_H
#define TEST_EF_CUSTOM_CONFIGURATOR_H

#include <map>
#include <string>

#include <ttl/include/EventFramework/Interfaces/IEventConfigurator.h>

//Just Stub configurator...
struct Configurator :
    public IEventConfigurator<Configurator>
{

    //Interface Impl
    template <class ConfigurationValueType>
    ConfigurationValueType getValueFromConfigurationImpl(const std::string &configurationName, const std::string &valueName) const
    {
        assert(configurationName == "ControllableEvents");

        auto it = configurationMap.find(valueName);
        if(it == configurationMap.end())
        {
            static const std::string empty;
            return empty;
        }
        return it->second;
    }

    template <class ConfigurationValueType>
    std::unordered_set<ConfigurationValueType> getMultipleValuesFromConfigurationImpl(const std::string &configurationName, const std::string &valueName) const
    {
        assert(configurationName == "ControllableEvents");

        std::unordered_set<std::string> ret;
        auto range = configurationMap.equal_range(valueName);
        for(auto it = range.first; it != range.second; ++it)
        {
            ret.insert(it->second);
        }
        return ret;
    }

    //Store configuration parameters here
    std::multimap<std::string, std::string> configurationMap;
};


#endif //TEST_EF_CUSTOM_CONFIGURATOR_H
