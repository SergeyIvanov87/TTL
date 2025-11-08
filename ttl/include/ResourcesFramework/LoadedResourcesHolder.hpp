#ifndef RESOURCESHOLDER_HPP_
#define RESOURCESHOLDER_HPP_

#include "LoadedResourcesHolder.h"
#include <ttl/include//Utils/CTimeUtils.h>
#include "BaseObjectLoader.hpp"

namespace Resources
{
#define TEMPLATE_ARGS_LIST_DECL  class ...Loaders
#define TEMPLATE_ARGS_LIST_DEF   Loaders...

template <TEMPLATE_ARGS_LIST_DECL>
LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::LoadedResourcesHolder(const std::string &assetsPath, const std::string &tmpOperationsPath) :
 m_assetsPath(assetsPath),
 m_assetsTmpPath(tmpOperationsPath)
{
    (void)getSerializationPath();
}

template <TEMPLATE_ARGS_LIST_DECL>
LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::~LoadedResourcesHolder()
{
    deinitResourceLoader();
};

//function to get specific resource from specific resource loader
template <TEMPLATE_ARGS_LIST_DECL>
template <class Resource>
const Resource *LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::getResourcePtr(std::string_view name, bool needDeserialize/* = false*/)
{
    auto ret = std::get<BaseObjectLoader<Resource>>(loadersTuple).getResourceByName(name);
    if(ret)
    {
        if(ret->wasSerialized() && needDeserialize)
        {
            if(deserializeResource<Resource>(name))
            {
                return ret;
            }
            return nullptr;
        }
        return ret;
    }
    return nullptr;
}

template <TEMPLATE_ARGS_LIST_DECL>
template <class Resource>
Resource *LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::getResourceInstancePtr(std::string_view name, bool needDeserialize/* = false*/)
{
    auto ret = std::get<BaseObjectLoader<Resource>>(loadersTuple).getResourceByName(name);
    if(ret)
    {
        if(ret->wasSerialized() && needDeserialize)
        {
            if(deserializeResource<Resource>(name))
            {
                return ret;
            }
            return nullptr;
        }
        return ret;
    }
    return nullptr;
}

//function to set specific resource to specific resource loader
template <TEMPLATE_ARGS_LIST_DECL>
template <class Resource>
bool LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::insertResource(std::string_view name, std::shared_ptr<Resource> &&resourcePtr)
{
    return std::get<BaseObjectLoader<Resource>>(loadersTuple).setResourceByName(name,
                                            std::forward<typename BaseObjectLoader<Resource>::ResourceClassTypeSharedPtr>(resourcePtr));
}

//function to de/serialize object into specific file
template <TEMPLATE_ARGS_LIST_DECL>
template <class Resource>
bool LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::serializeResource(std::string_view name)
{
    //Enter to main resources tree directory
    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    if(chdir(getAssetsPath().c_str()) != 0)
    {
        throw urc::SystemError(urc::ResultCodes::RESULT_SYSTEM_RESOURCE_LOCATION_ERROR,
                              getAssetsPath(), ": ",
                              strerror(errno),
                              ". Current dir: ",
                               curDirPtr.get());
    }
    return std::get<BaseObjectLoader<Resource>>(loadersTuple).serialize(name);
}

template <TEMPLATE_ARGS_LIST_DECL>
template <class Resource>
bool LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::deserializeResource(std::string_view name)
{
    //Enter to main resources tree directory
    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    if(chdir(getAssetsPath().c_str()) != 0)
    {
        throw urc::SystemError(urc::ResultCodes::RESULT_SYSTEM_RESOURCE_LOCATION_ERROR,
                              getAssetsPath(), ": ",
                              strerror(errno),
                              ". Current dir: ",
                               curDirPtr.get());
    }
    return std::get<BaseObjectLoader<Resource>>(loadersTuple).deserialize(name);
}

//load resources for all ResourceLoaders
template <TEMPLATE_ARGS_LIST_DECL>
template <class UsedTracer>
bool LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::initResourceLoader(UsedTracer tracer)
{
    //Enter to main resources tree directory
    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    if(chdir(getAssetsPath().c_str()) != 0)
    {
        throw urc::SystemError(urc::ResultCodes::RESULT_SYSTEM_RESOURCE_LOCATION_ERROR,
                              getAssetsPath(), ": ",
                              strerror(errno),
                              ". Current dir: ",
                               curDirPtr.get());
    }

    tracer.trace("Total resource types ", std::tuple_size_v<ResourceLoadersTuple>, " for loading:\n");
    CTimeUtils::for_each_in_tuple(loadersTuple, [&tracer](size_t index, auto &x)
    {
        x.loadResources(tracer);
    });
    return true;
}

//free resources for all ResourceLoaders
template <TEMPLATE_ARGS_LIST_DECL>
bool LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::deinitResourceLoader()
{
    CTimeUtils::for_each_in_tuple(loadersTuple, [](size_t index, auto &x)
    {
        x.freeResources();
    });
    return true;
}

template <TEMPLATE_ARGS_LIST_DECL>
const std::string &LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::getAssetsPath() const
{
    return m_assetsPath;
}

template <TEMPLATE_ARGS_LIST_DECL>
const std::string &LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::getSerializationPath() const
{
    return m_assetsTmpPath;
}

#undef TEMPLATE_ARGS_LIST_DECL
#undef TEMPLATE_ARGS_LIST_DEF
} /* namespace Resources */

#endif /* RESOURCESHOLDER_H_ */
