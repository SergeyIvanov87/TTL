#ifndef IRESOURCELOADER_H
#define IRESOURCELOADER_H
#include <memory>
#include <string>
#include <map>
#include <memory>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "FrameworkSpecializations/ResourcesFrameworkSpecialization/ResourcesTypes.h"
#include "Framework/Utils/UnifiedResultCodes.h"

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

    typedef SharedPtr ResourceClassTypeSharedPtr;
    typedef const Resource * ResourceClassTypeCPtr;

    IBaseResource()
    {
        logger = log4cplus::Logger::getInstance(this->getResourceTypeDescription());
    }

    //Interfaces Decl
    static ResourcesMap loadResources(const std::string &resourcePath)
    {
        return ResourceClassType::loadResourcesImpl(resourcePath);
    }

    constexpr static const char *getResourceTypeDescription()
    {
        return ResourceClassType::getResourceTypeDescriptionImpl();
    }

protected:
    log4cplus::Logger logger;
};
}
#endif //IRESOURCELOADER_H
