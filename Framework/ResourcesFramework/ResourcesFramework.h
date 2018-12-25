#ifndef RESOURCES_FRAMEWORK_H
#define RESOURCES_FRAMEWORK_H

#include "Framework/ResourcesFramework/LoadedResourcesHolder.h"
namespace Resources
{
template <class ...RegisteredResourceTypes>
struct ResourcesFrameworkFactory
{
    using ResourceHolder = LoadedResourcesHolder<RegisteredResourceTypes...>;
    using ResourceHolderPtr = std::unique_ptr<ResourceHolder>;

    static ResourceHolder* Instance();
    static bool initializeResourceHolder(const std::string &assetsPath, const std::string &objectSerializationPath);
    static bool deinitializeResourceHolder();
private:
    static ResourceHolderPtr resourceHolderPtr;
/*
    template <class Type>
    typename IBaseResource<Type>::ResourceClassTypeCPtr getResourceObjectCPtr(const std::string &resourceName, bool needDeserialize = false)
    {
        auto ret = getResourceHolderPtr()->getResourceById<IBaseResource<Type>::ResourceClassType::getResourceType()>(resourceName);
        if(ret)
        {
            if(needDeserialize)
            {
                if(getResourceHolderPtr()->deserializeResource<IBaseResource<Type>::ResourceClassType::getResourceType()>(resourceName))
                {
                    return ret;
                }
                return nullptr;
            }
            return ret;
        }
        return nullptr;
    }
*/
/*
template <class Type>
bool serializeResourceObject(const std::string &resourceName)
{
    return getResourceHolderPtr()->serializeResource <BaseResource<Type>::ResourceClassType::getResourceType()>(resourceName);
}

template <class Type>
bool deserializeResourceObject(const std::string &resourceName)
{
    return getResourceHolderPtr()->deserializeResource <BaseResource<Type>::ResourceClassType::getResourceType()>(resourceName);
}
*/
/*
    template <class Type>
    bool setResourceObject(
                        const std::string &resourceName,
                        const typename IBaseResource<Type>::ResourceClassTypeSharedPtr &sharedPtr)
    {
        return getResourceHolderPtr()->setResourceById <BaseResource<Type>::ResourceClassType::getResourceType()>(resourceName, sharedPtr);
    }
    */
};

template <class ...RegisteredResourceTypes>
typename ResourcesFrameworkFactory<RegisteredResourceTypes...>::ResourceHolderPtr ResourcesFrameworkFactory<RegisteredResourceTypes...>::resourceHolderPtr;
}
#endif //RESOURCES_FRAMEWORK_H
