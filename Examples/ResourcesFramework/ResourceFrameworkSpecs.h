#ifndef RESOURCES_FRAMEWORK_SPECS_H
#define RESOURCES_FRAMEWORK_SPECS_H

#include "CustomResourceLoaders.h"
#include "Framework/ResourcesFramework/ResourcesFramework.h"
#include "Framework/ResourcesFramework/BaseObjectLoader.h"

extern const char resourceA_path_str[];
using ResourceALoader = Resources::BaseObjectLoader<ResourceA, resourceA_path_str>;


using ResourcesFramework = Resources::ResourcesFrameworkFactory<ResourceALoader>;

#endif //RESOURCES_FRAMEWORK_SPECS_H
