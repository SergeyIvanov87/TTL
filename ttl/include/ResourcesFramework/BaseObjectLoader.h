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
    using OwningPtr = typename ResourceHolder::OwningPtr;
    using OwningPtrConst = typename ResourceHolder::OwningPtrConst;
    using NonOwningPtrConst = typename ResourceHolder::NonOwningPtrConst;
    using NonOwningPtr = typename ResourceHolder::NonOwningPtr;
    using ResourcesMap = typename ResourceHolder::ResourcesMap;

    constexpr static const char *getResourceTypeDescription()
    {
        return ResourceHolder::getResourceTypeDescription();
    };

    OwningPtrConst getResourceByName(std::string_view name) const;
    OwningPtr getResourceByName(std::string_view name);

    NonOwningPtrConst getNonOwnResourceByName(std::string_view name) const;
    NonOwningPtr getNonOwnResourceByName(std::string_view name);

    bool setResourceByName(std::string_view name,
                           const OwningPtr &resource);

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
    bool doSerialize(std::string_view resourceName, OwningPtr &resource, UsedTracer tracer = UsedTracer());

    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    bool doDeserialize(std::string_view resourceName, OwningPtr &resource, UsedTracer tracer = UsedTracer());
};
}
#endif /* BASEOBJECTLOADER_H_ */
