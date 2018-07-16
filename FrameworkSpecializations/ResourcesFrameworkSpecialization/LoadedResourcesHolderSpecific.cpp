#define CONCRETE_CPP
#include "LoadedResourcesHolderSpecific.h"
#include "Framework/ResourcesFramework/LoadedResourcesHolderImpl.hpp"
#include "Framework/ResourcesFramework/LoadedResourcesHolder.hpp"
#undef CONCRETE_CPP

namespace Resources
{
template class LoadedResourcesHolder<
                                        SystemInfoObjectLoader,
                                        ModelFileDescriptionObjectLoader,
                                        ShaderObjectLoader,
                                        ShaderProgramObjectLoader,
                                        MeshObjectLoader,
                                        MaterialObjectLoader,
                                        TexturesObjectLoader>;

extern ResourceHolderPtr resourceHolderPtr;

void createResourceHolder()
{
    resourceHolderPtr = std::make_shared<ResourceHolder>();
    resourceHolderPtr->initResourceLoader();
}


#define INSTANTIATE_getResourceObjectCPtr(Loader) template typename Loader::ResourceClassTypeCPtr getResourceObjectCPtr<typename Loader::ResourceClassType>(const std::string &resourceName, bool needDeserialize = false)

INSTANTIATE_getResourceObjectCPtr(SystemInfoObjectLoader);
INSTANTIATE_getResourceObjectCPtr(ModelFileDescriptionObjectLoader);
INSTANTIATE_getResourceObjectCPtr(ShaderObjectLoader);
INSTANTIATE_getResourceObjectCPtr(ShaderProgramObjectLoader);
INSTANTIATE_getResourceObjectCPtr(MeshObjectLoader);
INSTANTIATE_getResourceObjectCPtr(MaterialObjectLoader);
INSTANTIATE_getResourceObjectCPtr(TexturesObjectLoader);

#define INSTANTIATE_setResourceObject(Type) template bool setResourceObject<Type>(const std::string &resourceName,const typename BaseResource<Type>::ResourceClassTypeSharedPtr &sharedPtr)

INSTANTIATE_setResourceObject(typename SystemInfoObjectLoader::ResourceClassType);
INSTANTIATE_setResourceObject(typename ModelFileDescriptionObjectLoader::ResourceClassType);
INSTANTIATE_setResourceObject(typename ShaderObjectLoader::ResourceClassType);
INSTANTIATE_setResourceObject(typename ShaderProgramObjectLoader::ResourceClassType);
INSTANTIATE_setResourceObject(typename MeshObjectLoader::ResourceClassType);
INSTANTIATE_setResourceObject(typename MaterialObjectLoader::ResourceClassType);
INSTANTIATE_setResourceObject(typename TexturesObjectLoader::ResourceClassType);
}
