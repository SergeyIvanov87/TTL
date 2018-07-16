/*
 * ResourcesHolder.h
 *
 *  Created on: Feb 11, 2016
 *      Author: user
 */

#ifndef RESOURCESHOLDER_H_
#define RESOURCESHOLDER_H_
#include <tuple>
#include <array>
#include <utility>
#include <unistd.h>
#include <iostream>
#include <error.h>
#include <string.h>
#include "FrameworkSpecializations/ResourcesFrameworkSpecialization/ResourcesTypes.h"
#include "../Utils/CTimeUtils.h"


//!!!!  DELETE IT #include "BaseObjectLoaderConcrete.h"
namespace Resources {

//instantiated by Resource Loader template classes
template <class ... Loaders>
class LoadedResourcesHolder {
public:
    typedef std::tuple<Loaders...> ResourceLoadersTuple;

    ~LoadedResourcesHolder();

    //function to get specific resource from specific resource loader
    template<Resources::ResourceType type>
    typename std::tuple_element<type, ResourceLoadersTuple>::type::ResourceClassTypeCPtr
    getResourceById(const std::string &name) const;

    //function to set specific resource to specific resource loader
    template<Resources::ResourceType type>//, typename std::tuple_element<type, ResourceLoadersTuple>::type::ResourceClassTypeCPtr>
    bool setResourceById(const std::string &name,
        const typename std::tuple_element<type, ResourceLoadersTuple>::type::ResourceClassTypeSharedPtr &resource);

    //function to de/serialize object into specific file
    template<Resources::ResourceType type>
    bool serializeResource(const std::string &name);
    template<Resources::ResourceType type>
    bool deserializeResource(const std::string &name);
private:
    ResourceLoadersTuple loadersTuple;
public:
    //load resources for all ResourceLoaders
    bool initResourceLoader();
    //free resources for all ResourceLoaders
    bool deinitResourceLoader();
};
} /* namespace Resources */

#endif /* RESOURCESHOLDER_H_ */
