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
    using Self = BaseObjectLoader;
public:
    using ResourceClassType = typename ResourceHolder::ResourceClassType;
    using OwnPtr = typename ResourceHolder::OwnPtr;
    using OwnPtrConst = typename ResourceHolder::OwnPtrConst;
    using NonOwnPtrConst = typename ResourceHolder::NonOwnPtrConst;
    using NonOwnPtr = typename ResourceHolder::NonOwnPtr;
    using ResourcesMap = typename ResourceHolder::ResourcesMap;

    constexpr static const char *getResourceTypeDescription()
    {
        return ResourceHolder::getResourceTypeDescription();
    };

    OwnPtrConst getResourceByName(std::string_view name) const;
    OwnPtr getResourceByName(std::string_view name);

    NonOwnPtrConst getNonOwnResourceByName(std::string_view name) const;
    NonOwnPtr getNonOwnResourceByName(std::string_view name);

    bool setResourceByName(std::string_view name,
                           const OwnPtr &resource);

    void freeResources();

    template <class UsedTracer>
    size_t loadResources(UsedTracer tracer);

    size_t size() const;

    bool serialize(std::string_view resourceName);
    bool deserialize(std::string_view resourceName);
private:

    template <class UsedTracer>
    size_t doLoadResourcesFromFS(UsedTracer &tracer);
    template <class UsedTracer>
    size_t doLoadResourcesFromMemory(UsedTracer &tracer);
    ResourcesMap loadedObjectResources;

    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    bool doSerialize(std::string_view resourceName, OwnPtr &resource, UsedTracer tracer = UsedTracer());

    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    bool doDeserialize(std::string_view resourceName, OwnPtr &resource, UsedTracer tracer = UsedTracer());
};
}
#endif /* BASEOBJECTLOADER_H_ */
