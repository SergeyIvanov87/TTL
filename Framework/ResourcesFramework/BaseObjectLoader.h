#ifndef BASEOBJECTLOADER_H_
#define BASEOBJECTLOADER_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <fstream>

#include <Framework/Utils/LogTracer.h>

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

    template <class UsedTracer>
    size_t loadResources(UsedTracer tracer);

    bool serialize(const std::string &resourceName);
    bool deserialize(const std::string &resourceName);
private:

    template <class UsedTracer>
    size_t doLoadResourcesFromFS(UsedTracer &tracer);
    template <class UsedTracer>
    size_t doLoadResourcesFromMemory(UsedTracer &tracer);
    ResourcesMap loadedObjectResources;

    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    bool doSerialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource, UsedTracer tracer = UsedTracer());

    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    bool doDeserialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource, UsedTracer tracer = UsedTracer());
};
}
#endif /* BASEOBJECTLOADER_H_ */
