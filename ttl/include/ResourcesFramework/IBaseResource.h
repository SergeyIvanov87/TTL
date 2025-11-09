#ifndef IRESOURCELOADER_H
#define IRESOURCELOADER_H

#include <memory>
#include <string>
#include <map>
#include <memory>
#include <ttl/include/Utils/UnifiedResultCodes.h>

namespace Resources
{
struct BaseResourceTag{};
template<typename T>
concept BaseResourceConcept = requires (T p) {T::getResourceTypeDescriptionImpl(); p.loadResourcesImpl("string", "");} &&
    std::derived_from<T, BaseResourceTag>;

// TODO https://www.sandordargo.com/blog/2024/12/04/crtp-vs-concepts
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
    typedef Resource * ResourceClassTypePtr;

    //Interfaces Decl
    template<class Tracer, class ...Additionals>
    static ResourcesMap loadResources(const std::string &resourcePath, Tracer &&tracer, Additionals &&...args)
    {
        return ResourceClassType::loadResourcesImpl(resourcePath,
                                  std::forward<Tracer>(tracer),
                                  std::forward<Additionals>(args)...);
    }

    constexpr static const char *getResourceTypeDescription()
    {
        return ResourceClassType::getResourceTypeDescriptionImpl();
    }
};
}
#endif //IRESOURCELOADER_H
