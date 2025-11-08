#ifndef RESOURCES_FRAMEWORK_HPP
#define RESOURCES_FRAMEWORK_HPP
#include "ResourcesFramework.h"
#include <ttl/include//ResourcesFramework/LoadedResourcesHolder.hpp>

namespace Resources
{
#define TEMPLATE_ARGS_LIST_DECL  class ...RegisteredResourceTypes
#define TEMPLATE_ARGS_LIST_DEF   RegisteredResourceTypes...

template <TEMPLATE_ARGS_LIST_DECL>
typename ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::ResourceHolder *
        ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::Instance()
{
    return ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::resourceHolderPtr.get();
}

template <TEMPLATE_ARGS_LIST_DECL>
template <class UsedTracer>
bool ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::initializeResourceHolder(const std::string &assetsPath, const std::string &objectSerializationPath, UsedTracer tracer)
{
    auto ret = Instance();
    if(!ret)
    {
        ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::resourceHolderPtr = std::make_unique<ResourceHolder>(assetsPath, objectSerializationPath);
        return ResourcesFrameworkFactory<TEMPLATE_ARGS_LIST_DEF>::resourceHolderPtr->template initResourceLoader(tracer);
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
#endif //RESOURCES_FRAMEWORK_HPP
