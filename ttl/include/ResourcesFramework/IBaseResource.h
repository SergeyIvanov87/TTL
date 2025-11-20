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
class IBaseResource: public Resources::BaseResourceTag
{
public:
    //Export Defines
    using ResourceClassType = Resource;
    using OwnPtr = std::shared_ptr<ResourceClassType>;
    using OwnPtrConst = std::shared_ptr<const ResourceClassType>;
    using NonOwnPtr = std::weak_ptr<ResourceClassType>;
    using NonOwnPtrConst = std::weak_ptr<const ResourceClassType>;
    using ResourcesMap = std::map<std::string, OwnPtr>;

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
