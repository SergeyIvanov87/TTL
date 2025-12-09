#ifndef IRESOURCELOADER_H
#define IRESOURCELOADER_H

#include <memory>
#include <string>
#include <map>
#include <memory>
#include <ttl/Utils/UnifiedResultCodes.h>

namespace Resources
{
struct BaseResourceTag{};
template<typename T>
concept BaseResourceConcept = requires (T p) {T::getResourceTypeDescriptionImpl(); p.loadResourcesImpl("string", "");} &&
    std::derived_from<T, BaseResourceTag>;

// TODO https://www.sandordargo.com/blog/2024/12/04/crtp-vs-concepts
template<class Resource>
class IBaseResource: public Resources::BaseResourceTag
{
public:
    //Export Defines
    using ResourceClassType = Resource;
    using OwningPtr = std::shared_ptr<ResourceClassType>;
    using OwningPtrConst = std::shared_ptr<const ResourceClassType>;
    using NonOwningPtr = std::weak_ptr<ResourceClassType>;
    using NonOwningPtrConst = std::weak_ptr<const ResourceClassType>;
    using ResourcesMap = std::map<std::string, OwningPtr>;

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
