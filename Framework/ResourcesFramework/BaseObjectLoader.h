#ifndef BASEOBJECTLOADER_H_
#define BASEOBJECTLOADER_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "ResourceTraits.h"
#include "IBaseResource.h"
#include "../Utils/Utils.h"

namespace Resources
{
template<class ResourceHolder>
class BaseObjectLoader final
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

    constexpr static const char *getResourceTypeDescription()
    {
        return ResourceHolder::getResourceTypeDescription();
    };

    ResourceClassTypeCPtr getResourceByName(const std::string &name) const;
    bool setResourceByName(const typename ResourcesMap::key_type &name,
            const ResourceClassTypeSharedPtr &resource);

    void freeResources();
    bool loadResources();

    bool serialize(const std::string &resourceName);
    bool deserialize(const std::string &resourceName);
protected:
    log4cplus::Logger logger;
private:

    bool doLoadResourcesFromFS();
    bool doLoadResourcesFromMemory();
    ResourcesMap loadedObjectResources;

    bool doSerialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource);
    bool doDeserialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource);
};
}
#endif /* BASEOBJECTLOADER_H_ */
