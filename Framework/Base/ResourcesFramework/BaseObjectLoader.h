/*
 * VerticesObjectHolder.h
 *
 *  Created on: Feb 5, 2016
 *      Author: user
 */

#ifndef BASEOBJECTLOADER_H_
#define BASEOBJECTLOADER_H_

#include "BaseResource.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "../Utils/Utils.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

template <bool B>
struct invoker {};

template <>
struct invoker<true>
{
    template <class T>
    static bool invoke(T *t)
    {
        return t->loadResourcesByEmptyPath();
    }
};

template <>
struct invoker<false>
{
    template <class T>
    static bool invoke(T *t)
    {
        return t->loadResources();
    }
};

namespace Resources
{
template<class ResourceHolder, const char *treeLeafPath = system_info_dummy_path>
class BaseObjectLoader
{
public:
    ~BaseObjectLoader() = default;
    BaseObjectLoader();

public:
    typedef typename ResourceHolder::ResourceClassType ResourceClassType;
    typedef typename ResourceHolder::ResourceClassTypeSharedPtr ResourceClassTypeSharedPtr;
    typedef typename ResourceHolder::ResourceClassTypeCPtr ResourceClassTypeCPtr;
    typedef typename ResourceHolder::ResourcesMap ResourcesMap;
    typedef typename ResourceHolder::ResourcesMapValueType ResourcesMapValueType;
    typedef typename ResourceHolder::ResourcesMapIt ResourcesMapIt;
    typedef typename ResourceHolder::ResourcesMapCIt ResourcesMapCIt;
    constexpr static const char* ResourceTreePath = treeLeafPath;

    ResourceClassTypeCPtr getResourceByName(const std::string &name) const;
    typename ResourceHolder::ResourceType getResourceType() const
    {
        return ResourceHolder::resourceType;
    };
    const char *getResourceTreePath() const
    {
        return treeLeafPath;
    };

    bool setResourceByName(const typename ResourcesMap::key_type &name,
            const ResourceClassTypeSharedPtr &resource);
    void freeResources();
    bool loadResources();

    bool serialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource);
    bool serialize(const std::string &resourceName);

    bool deserialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource);
    bool deserialize(const std::string &resourceName);
protected:
    log4cplus::Logger logger;
private:
    ResourcesMap loadedObjectResources;
};

////////////////////////////////////////////////////////////////////////////////
//partial specialization for dummy file path
template<class ResourceHolder>
class BaseObjectLoader<ResourceHolder, system_info_dummy_path> {
public:
    ~BaseObjectLoader() = default;
    BaseObjectLoader();

public:
    typedef typename ResourceHolder::ResourceClassType ResourceClassType;
    typedef typename ResourceHolder::ResourceClassTypeSharedPtr ResourceClassTypeSharedPtr;
    typedef typename ResourceHolder::ResourceClassTypeCPtr ResourceClassTypeCPtr;
    typedef typename ResourceHolder::ResourcesMap ResourcesMap;
    typedef typename ResourceHolder::ResourcesMapValueType ResourcesMapValueType;
    typedef typename ResourceHolder::ResourcesMapIt ResourcesMapIt;
    typedef typename ResourceHolder::ResourcesMapCIt ResourcesMapCIt;

    ResourceClassTypeCPtr getResourceByName(const std::string &name) const;
    typename ResourceHolder::ResourceType getResourceType() const
    {
        return ResourceHolder::resourceType;
    };

    bool setResourceByName(const typename ResourcesMap::key_type &name,
            const ResourceClassTypeSharedPtr &resource);
    void freeResources();
    bool loadResources();

    bool serialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource);
    bool serialize(const std::string &resourceName);

    bool deserialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource);
    bool deserialize(const std::string &resourceName);
protected:
    log4cplus::Logger logger;
private:
    ResourcesMap loadedObjectResources;
};
////////////////////////////////////////////////////////////////////////////////


}
#endif /* BASEOBJECTLOADER_H_ */
