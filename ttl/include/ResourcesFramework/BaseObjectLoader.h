#ifndef BASEOBJECTLOADER_H_
#define BASEOBJECTLOADER_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <string_view>
#include <fstream>

#include <ttl/include/Utils/LogTracerAdapter.h>

#include "ResourceTraits.h"
#include "IBaseResource.h"
#include "../Utils/Utils.h"

namespace Resources
{
template<class ResourceHolder>
class BaseObjectLoader final
{
public:
    typedef typename ResourceHolder::ResourceClassType ResourceClassType;
    typedef typename ResourceHolder::ResourceClassTypeSharedPtr ResourceClassTypeSharedPtr;
    typedef typename ResourceHolder::ResourceClassTypeCPtr ResourceClassTypeCPtr;
    typedef typename ResourceHolder::ResourceClassTypePtr ResourceClassTypePtr;
    typedef typename ResourceHolder::ResourcesMap ResourcesMap;
    typedef typename ResourceHolder::ResourcesMapValueType ResourcesMapValueType;

    constexpr static const char *getResourceTypeDescription()
    {
        return ResourceHolder::getResourceTypeDescription();
    };

    ResourceClassTypeCPtr getResourceByName(std::string_view name) const;
    ResourceClassTypePtr getResourceByName(std::string_view name);
    bool setResourceByName(std::string_view name,
                           const ResourceClassTypeSharedPtr &resource);

    void freeResources();

    template <class UsedTracer>
    size_t loadResources(UsedTracer tracer);

    bool serialize(std::string_view resourceName);
    bool deserialize(std::string_view resourceName);
private:

    template <class UsedTracer>
    size_t doLoadResourcesFromFS(UsedTracer &tracer);
    template <class UsedTracer>
    size_t doLoadResourcesFromMemory(UsedTracer &tracer);
    ResourcesMap loadedObjectResources;

    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    bool doSerialize(std::string_view resourceName, ResourceClassTypeSharedPtr &resource, UsedTracer tracer = UsedTracer());

    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    bool doDeserialize(std::string_view resourceName, ResourceClassTypeSharedPtr &resource, UsedTracer tracer = UsedTracer());
};
}
#endif /* BASEOBJECTLOADER_H_ */
