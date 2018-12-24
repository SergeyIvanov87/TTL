#ifndef CUSTOM_RESOURCE_LOADERS_H
#define CUSTOM_RESOURCE_LOADERS_H
extern const char system_info_dummy_path[];

#include <Framework/ResourcesFramework/BaseObjectLoader.h>
#include <Framework/Utils/StringUtils.h>

class ResourceA : public Resources::IBaseResource<ResourceA>
{
public:
    using Base = Resources::IBaseResource<ResourceA>;
    using Base::ResourcesMap;
    static constexpr size_t instancesForLoadingCount = 10;

    ResourceA(const std::string &name) :
        Base(),
        m_name(name)
    {
    }

    //Interfaces Decl
    static ResourcesMap loadResourcesImpl(const std::string &resourcePath)
    {
        ResourcesMap instances;
        for (size_t i = 0; i < instancesForLoadingCount; i ++)
        {
            std::string aggregatedName = makeString(getResourceTypeDescriptionImpl(), "_", i);
            instances.emplace(aggregatedName, std::make_shared<ResourceA>(aggregatedName));
        }
        return instances;
    }

    constexpr static const char *getResourceTypeDescriptionImpl()
    {
        return "ResourceA";
    }

    std::string m_name;
};


#endif //CUSTOM_RESOURCE_LOADERS_H
