#ifndef LOADEDRESOURCESHOLDERIMPL_HPP
#define LOADEDRESOURCESHOLDERIMPL_HPP

#include "FrameworkSpecializations/ResorcesFrameworkSpecialization/LoadedResourcesHolderSpecific.h"

namespace Resources
{
extern ResourceHolderPtr resourceHolderPtr;

/*
template <class ...Resources>
void createResourceHolder(Resources ...args)
{
    resourceHolderPtr = std::make_shared<ResourceHolder>(args...);
}
*/

template <class Type>
typename BaseResource<Type>::ResourceClassTypeCPtr getResourceObjectCPtr(const std::string &resourceName, bool needDeserialize = false)
{
    typename BaseResource<Type>::ResourceClassTypeCPtr ret =
        getResourceHolderPtr()->getResourceById <BaseResource<Type>::ResourceClassType::getResourceType()>(resourceName);
    if(ret)
    {
        if(needDeserialize)
        {
            if(getResourceHolderPtr()->deserializeResource<BaseResource<Type>::ResourceClassType::getResourceType()>(resourceName))
            {
                return ret;
            }
            return nullptr;
        }
        return ret;
    }
    return nullptr;
}

template <class Type>
bool serializeResourceObject(const std::string &resourceName)
{
    return getResourceHolderPtr()->serializeResource <BaseResource<Type>::ResourceClassType::getResourceType()>(resourceName);
}

template <class Type>
bool derializeResourceObject(const std::string &resourceName)
{
    return getResourceHolderPtr()->deserializeResource <BaseResource<Type>::ResourceClassType::getResourceType()>(resourceName);
}

template <class Type>
bool setResourceObject(
    const std::string &resourceName,
    const typename BaseResource<Type>::ResourceClassTypeSharedPtr &sharedPtr)
{
    return getResourceHolderPtr()->setResourceById <BaseResource<Type>::ResourceClassType::getResourceType()>(resourceName, sharedPtr);
}
}


#endif /* LOADEDRESOURCESHOLDERIMPL_HPP */
