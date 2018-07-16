#ifndef IRESOURCELOADER_H
#define IRESOURCELOADER_H
#include <memory>
#include <string>
#include <map>
#include <memory>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include "ResourcesTypes.h"
#include "../Utils/ErrorCodes.h"

namespace Resources
{

template<class Resource>
class IBaseResource
{
public:
    //Export Defines
    typedef Resource ResourceClassType;
    typedef std::shared_ptr<ResourceClassType> SharedPtr;
    typedef std::map<std::string, SharedPtr> ResourcesMap;
    typedef typename ResourcesMap::value_type ResourcesMapValueType;
    typedef typename ResourcesMap::iterator ResourcesMapIt;

    typedef SharedPtr ResourceClassTypeSharedPtr;
    typedef const Resource * ResourceClassTypeCPtr;
    typedef typename ResourcesMap::const_iterator ResourcesMapCIt;
    //TODO
    typedef typename Resources::ResourceType ResourceType;
    constexpr static ResourceType resourceType = ResourceClassType::getResourceType();

    IBaseResource()
    {
        logger = log4cplus::Logger::getInstance(getResourceTypeString(resourceType));
    }

    //Interfaces Decl
    static ResourcesMap loadResources(const std::string &resourcePath)
    {
        return ResourceClassType::loadResourcesImpl(resourcePath);
    }

protected:
    log4cplus::Logger logger;
};
}
#endif //IRESOURCELOADER_H
