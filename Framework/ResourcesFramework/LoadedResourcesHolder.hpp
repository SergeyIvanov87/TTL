/*
 * ResourcesHolder.h
 *
 *  Created on: Feb 11, 2016
 *      Author: user
 */

#ifndef RESOURCESHOLDER_HPP_
#define RESOURCESHOLDER_HPP_
#include "LoadedResourcesHolder.h"


//!!!!  DELETE IT #include "BaseObjectLoaderConcrete.h"
namespace Resources {

//instantiated by Resource Loader template classes
template <class ... Loaders>
LoadedResourcesHolder<Loaders...>::~LoadedResourcesHolder()
{
    deinitResourceLoader();
};

//function to get specific resource from specific resource loader
template <class ... Loaders>
template<Resources::ResourceType type>
typename std::tuple_element<type, typename LoadedResourcesHolder<Loaders...>::ResourceLoadersTuple>::type::ResourceClassTypeCPtr
    LoadedResourcesHolder<Loaders...>::getResourceById(const std::string &name) const
    {
        return std::get<type>(loadersTuple).getResourceByName(name);
    }

//function to set specific resource to specific resource loader
template <class ... Loaders>
template<Resources::ResourceType type>
bool LoadedResourcesHolder<Loaders...>::setResourceById(const std::string &name,
        const typename std::tuple_element<type, ResourceLoadersTuple>::type::ResourceClassTypeSharedPtr &resource)
{
    return std::get<type>(loadersTuple).setResourceByName(name, resource);
}

//function to de/serialize object into specific file
template <class ... Loaders>
template<Resources::ResourceType type>
bool LoadedResourcesHolder<Loaders...>::serializeResource(const std::string &name)
    {
        //Enter to main reources tree directory
        char * curDir = get_current_dir_name();
        bool ret = false;
        do
        {
            if(chdir(main_resources_path) != 0)
            {
                std::cout << "Cannot enter in resource dir: " <<
                        strerror(errno) << ". Current dir: " << curDir<< std::endl;
                break;
            }
            ret =  std::get<type>(loadersTuple).serialize(name);
        }
        while(false);

        //Back current directory
        chdir(curDir);
        free(curDir);
        return ret;
    }
    template <class ... Loaders>
    template<Resources::ResourceType type>
    bool LoadedResourcesHolder<Loaders...>::deserializeResource(const std::string &name)
    {
        //Enter to main reources tree directory
        char * curDir = get_current_dir_name();
        bool ret = false;
        do
        {
            if(chdir(main_resources_path) != 0)
            {
                std::cout << "Cannot enter in resource dir: " <<
                        strerror(errno) << ". Current dir: " << curDir<< std::endl;
                break;
            }
            ret = std::get<type>(loadersTuple).deserialize(name);
        }
        while (false);

        //Back current directory
        chdir(curDir);
        free(curDir);
        return ret;
    }

    //load resources for all ResourceLoaders
    template <class ... Loaders>
    bool LoadedResourcesHolder<Loaders...>::initResourceLoader()
    {
        //Enter to main reources tree directory
        char * curDir = get_current_dir_name();

        do
        {
            if(chdir(main_resources_path) != 0)
            {
                std::cout << "Cannot enter in resource dir: " <<
                        strerror(errno) << ". Current dir: " << curDir<< std::endl;
                return false;
            }

            CTimeUtils::for_each_in_tuple(loadersTuple, [](size_t index, auto &x)
            {
                x.loadResources();
            });
        }
        while(false);

        //Back current directory
        chdir(curDir);
        free(curDir);
        return true;
    }

    //free resources for all ResourceLoaders
    template <class ... Loaders>
    bool LoadedResourcesHolder<Loaders...>::deinitResourceLoader()
    {
        CTimeUtils::for_each_in_tuple(loadersTuple, [](size_t index, auto &x)
        {
            x.freeResources();
        });
        return true;
    }
} /* namespace Resources */

#endif /* RESOURCESHOLDER_H_ */
