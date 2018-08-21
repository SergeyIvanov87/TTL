#ifndef RESOURCES_FRAMEWORK_HPP
#define RESOURCES_FRAMEWORK_HPP
#include "ResourcesFramework.h"
#include "LoadedResourcesHolderSpecific.h"
#include "Framework/ResourcesFramework/LoadedResourcesHolderImpl.hpp"
#include "Framework/ResourcesFramework/LoadedResourcesHolder.hpp"

namespace Resources
{
#define TEMPLATE_ARGS_LIST_DECL  class ...RegisteredResourceTypes
#define TEMPLATE_ARGS_LIST_DEF   RegisteredResourceTypes...

static typename ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::ResourceHolderPtr resourceHolderPtr;

template <TEMPLATE_ARGS_LIST_DECL>
ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::Instance()
{
    return resourceHolderPtr.get();
}

template <TEMPLATE_ARGS_LIST_DECL>
bool ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::initializeResourceHolder(const std::string &assetsPath, const std::string &objectSerializationPath)
{
    auto ret = Instance();
    if(!ret)
    {
        resourceHolderPtr = std::make_unique<ResourceHolder>(assetsPath, objectSerializationPath);
        return resourceHolderPtr->initResourceLoader();
    }
    return true;
}

template <TEMPLATE_ARGS_LIST_DECL>
bool ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::deinitializeResourceHolder()
{
    auto ret = Instance();
    {
        return ret->deinitResourceLoader();
    }
    return true;
}

#undef TEMPLATE_ARGS_LIST_DECL
#undef TEMPLATE_ARGS_LIST_DEF
}
#endif //RESOURCES_FRAMEWORK_H
