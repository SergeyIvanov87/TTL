#ifndef RESOURCES_FRAMEWORK_H
#define RESOURCES_FRAMEWORK_H

#include <ttl/ResourcesFramework/LoadedResourcesHolder.h>

namespace Resources
{
template <class ...RegisteredResourceTypes>
struct ResourcesFrameworkFactory
{
    using ResourceHolder = LoadedResourcesHolder<RegisteredResourceTypes...>;
    using ResourceHolderPtr = std::unique_ptr<ResourceHolder>;

    static ResourceHolder* Instance();

    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    static bool initializeResourceHolder(const std::string &assetsPath, const std::string &objectSerializationPath, UsedTracer tracer = UsedTracer());
    static bool deinitializeResourceHolder();
private:
    static ResourceHolderPtr resourceHolderPtr;
};

template <class ...RegisteredResourceTypes>
typename ResourcesFrameworkFactory<RegisteredResourceTypes...>::ResourceHolderPtr ResourcesFrameworkFactory<RegisteredResourceTypes...>::resourceHolderPtr;
}
#endif //RESOURCES_FRAMEWORK_H
