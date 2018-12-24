#ifndef LOADEDRESOURCESHOLDERCONCRETE_H_
#define LOADEDRESOURCESHOLDERCONCRETE_H_

#include "BaseObjectLoaderSpecific.h"
#include "../Framework/ResourcesFramework/LoadedResourcesHolder.h"

namespace Resources
{
#ifndef CONCRETE_CPP
extern template class LoadedResourcesHolder<
                                        SystemInfoObjectLoader,
                                        ModelFileDescriptionObjectLoader,
                                        ShaderObjectLoader,
                                        ShaderProgramObjectLoader,
                                        MeshObjectLoader,
                                        MaterialObjectLoader,
                                        TexturesObjectLoader>;
#endif

using ResourceHolder = LoadedResourcesHolder<
                                        SystemInfoObjectLoader,
                                        ModelFileDescriptionObjectLoader,
                                        ShaderObjectLoader,
                                        ShaderProgramObjectLoader,
                                        MeshObjectLoader,
                                        MaterialObjectLoader,
                                        TexturesObjectLoader>;

using ResourceHolderPtr = std::shared_ptr<ResourceHolder>;

bool isResourceHolderCreated();
void createResourceHolder();
ResourceHolder *getResourceHolderFacility();

inline ResourceHolder* getResourceHolderPtr()
{
    if(!isResourceHolderCreated())
    {
        createResourceHolder();
    }
    return getResourceHolderFacility();
}
}
#endif /* LOADEDRESOURCESHOLDERCONCRETE_H_ */
