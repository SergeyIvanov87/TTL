/*
 * ResourcesHolder.h
 *
 *  Created on: Feb 11, 2016
 *      Author: user
 */

#ifndef RESOURCESHOLDER_HPP_
#define RESOURCESHOLDER_HPP_
#include "LoadedResourcesHolder.h"

//!!!!  DELETE IT #include "BaseObjectLoaderConcrete.h"
namespace Resources
{
#define TEMPLATE_ARGS_LIST_DECL  class ...Loaders
#define TEMPLATE_ARGS_LIST_DEF   Loaders...

//instantiated by Resource Loader template classes

template <TEMPLATE_ARGS_LIST_DECL>
LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::LoadedResourcesHolder(const std::string &assetsPath, const std::string &tmpOperationsPath) :
 m_assetsPath(assetsPath),
 m_assetsTmpPath(tmpOperationsPath)
{
}

template <TEMPLATE_ARGS_LIST_DECL>
LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::~LoadedResourcesHolder()
{
    deinitResourceLoader();
};

//function to get specific resource from specific resource loader
template <TEMPLATE_ARGS_LIST_DECL>
template <class Resource>
const Resource *LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::getResourcePtr(const std::string &name, bool needDeserialize/* = false*/) const
{
    auto ret = std::get<Resource>(loadersTuple).getResourceByName(name);
    if(ret)
    {
        if(needDeserialize)
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
bool LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::insertResource(const std::string &name, std::shared_ptr<Resource> &&resourcePtr)
{
    return std::get<Resource>(loadersTuple).setResourceByName(name, std::forward<Resource>(resourcePtr));
}

//function to de/serialize object into specific file
template <TEMPLATE_ARGS_LIST_DECL>
template <class Resource>
bool LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::serializeResource(const std::string &name)
{
    //Enter to main resources tree directory
    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });
    if(chdir(m_assetsPath.c_str()) != 0)
    {
        std::cout << "Cannot enter in resource dir: " <<
                strerror(errno) << ". Current dir: " << curDirPtr.get()<< std::endl;
        return false;
    }
    return std::get<Resource>(loadersTuple).serialize(name);
}

template <TEMPLATE_ARGS_LIST_DECL>
template <class Resource>
bool LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::deserializeResource(const std::string &name)
{
    //Enter to main resources tree directory
    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });
    if(chdir(m_assetsPath.c_str()) != 0)
    {
        std::cout << "Cannot enter in resource dir: " <<
                strerror(errno) << ". Current dir: " << curDirPtr.get()<< std::endl;
        return false;
    }
    return std::get<Resource>(loadersTuple).deserialize(name);
}

//load resources for all ResourceLoaders
template <TEMPLATE_ARGS_LIST_DECL>
bool LoadedResourcesHolder<TEMPLATE_ARGS_LIST_DEF>::initResourceLoader()
{
    //Enter to main resources tree directory
    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    if(chdir(m_assetsPath.c_str()) != 0)
    {
        std::cout << "Cannot enter in resource dir: " <<
                strerror(errno) << ". Current dir: " << curDirPtr.get() << std::endl;
        return false;
    }

    CTimeUtils::for_each_in_tuple(loadersTuple, [](size_t index, auto &x)
    {
        x.loadResources();
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
