/*
 * VerticesObjectHolder.cpp
 *
 *  Created on: Feb 5, 2016
 *      Author: user
 */

//#include "BaseObjectLoader.h"
namespace Resources
{
template <class ResourceHolder,  const char *treeLeafPath>
BaseObjectLoader<ResourceHolder, treeLeafPath>::BaseObjectLoader()
{
    logger = log4cplus::Logger::getInstance(getResourceTypeString(BaseObjectLoader<ResourceHolder, treeLeafPath>::getResourceType()));
}

//Get/Set
template <class ResourceHolder,  const char *treeLeafPath>
typename BaseObjectLoader<ResourceHolder, treeLeafPath>::ResourceClassTypeCPtr BaseObjectLoader<ResourceHolder, treeLeafPath>::getResourceByName(const std::string &name) const
{
    ResourcesMapCIt it = loadedObjectResources.find(name);
    if(it != loadedObjectResources.end())
    {
        LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Get resource: ") << name <<
                LOG4CPLUS_TEXT(" Type: ") << getResourceTypeString(ResourceHolder::resourceType));
        return it->second.get();    //dereferenced shared ptr
    }
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Cannot get resource: ") << name <<
                LOG4CPLUS_TEXT(" Type: ") << getResourceTypeString(ResourceHolder::resourceType));
    return NULL;
}

template <class ResourceHolder,  const char *treeLeafPath>
bool BaseObjectLoader<ResourceHolder, treeLeafPath>::setResourceByName(
        const typename BaseObjectLoader<ResourceHolder, treeLeafPath>::ResourcesMap::key_type &name,
        const typename BaseObjectLoader<ResourceHolder, treeLeafPath>::ResourceClassTypeSharedPtr &resource)
{
    ResourcesMapIt it = loadedObjectResources.find(name);
    if(it == loadedObjectResources.end())
    {
        LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Insert resource: ") << name);
        loadedObjectResources.insert(std::make_pair(name, resource));
        return true;
    }
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Insert resource already exist: ") << name);
    return false;
}

//Free
template <class ResourceHolder,  const char *treeLeafPath>
void BaseObjectLoader<ResourceHolder, treeLeafPath>::freeResources()
{
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Free resources"));
    loadedObjectResources.clear();
}

//Deserialize
template <class ResourceHolder,  const char *treeLeafPath>
bool BaseObjectLoader<ResourceHolder, treeLeafPath>::deserialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource)
{
    if((!resource))
    {
        LOG4CPLUS_ERROR(logger,
            LOG4CPLUS_TEXT("Empty resource pointer for name: ") << resourceName);
        return false;
    }

    if(!resource->wasSerialized())
    {
        LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("resource was not serialized. skip it: ") << resourceName);
        return true;
    }
    char *curDir = get_current_dir_name();

    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Current directory: ") << curDir);
    chdir(getResourceTreePath());
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Change directory: ") << getResourceTreePath());

    bool result = false;
    do
    {
        //change to tmpDirectory
        if(-1 == chdir(tmpDirectory))
        {
            LOG4CPLUS_ERROR(logger,
                LOG4CPLUS_TEXT("Cannot change serialize directory: ") << curDir
                << LOG4CPLUS_TEXT("/")
                << getResourceTreePath()
                << LOG4CPLUS_TEXT("/")
                << tmpDirectory);

            throw urc::FileOpenError(std::string(curDir) + getResourceTreePath() + tmpDirectory, errno);
        }

        //open file for reading
        std::string fileName = resourceName + ".dump";
        LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Open file name: ") <<
                                fileName <<
                                LOG4CPLUS_TEXT(" to deserialize resource"));
        do
        {
            std::ifstream fileIn(fileName, std::ios::in | std::ios::binary);
            if(!fileIn.is_open())
            {
                LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Cannot open file for reading: ") << strerror(errno));
                throw urc::FileOpenError(fileName, errno);
            }
            result = resource->deserialize(fileIn);
            fileIn.close();
        } while(false);
    }while(false);

    chdir(curDir);
    free(curDir);
    return result;
}

template <class ResourceHolder,  const char *treeLeafPath>
bool BaseObjectLoader<ResourceHolder, treeLeafPath>::deserialize(const std::string &resourceName)
{
    ResourcesMapIt it = loadedObjectResources.find(resourceName);
    if(it == loadedObjectResources.end())
    {
        LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Cannot deserialize resource, not found: ") << resourceName);
        return false;
    }
    return deserialize(resourceName, it->second);
}

//Serialize
template <class ResourceHolder,  const char *treeLeafPath>
bool BaseObjectLoader<ResourceHolder, treeLeafPath>::serialize(
    const std::string &resourceName,
    ResourceClassTypeSharedPtr &resource)
{
    bool result = false;
    if((!resource))
    {
        LOG4CPLUS_ERROR(logger,
            LOG4CPLUS_TEXT("Empty resource pointer for name: ") << resourceName);
        return result;
    }

    char *curDir = get_current_dir_name();
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Current directory: ") << curDir);
    chdir(getResourceTreePath());
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Change directory: ") << getResourceTreePath());
    do
    {
        //change to tmpDirectory
        if(-1 == chdir(tmpDirectory))
        {
            if((errno != ENOENT) || (-1 == mkdir(tmpDirectory, S_IRWXO | S_IRWXG | S_IRWXU | S_IFDIR)))
            {
                LOG4CPLUS_ERROR(logger,
                    LOG4CPLUS_TEXT("Cannot create serialize directory: ") << curDir
                    << LOG4CPLUS_TEXT("/")
                    << getResourceTreePath()
                    << LOG4CPLUS_TEXT("/")
                    << tmpDirectory);

                throw urc::FileOpenError(std::string(curDir) + getResourceTreePath() + tmpDirectory, errno);
            }
        }

        //open file for dump
        std::string fileName = resourceName + ".dump";
        LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Open file name: ") <<
                            fileName <<
                            LOG4CPLUS_TEXT(" to dump resource"));

        do
        {
            std::ofstream fileOut(fileName, std::ios::out | std::ios::binary);
            if(!fileOut.is_open())
            {
                LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Cannot open file for writing"));
                throw urc::FileOpenError(fileName, errno);
            }
            LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Object size before serialize: ") << sizeof(*resource));
            result = resource->serialize(fileOut);
            LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Object size after serialize: ") << sizeof(*resource));
            fileOut.close();
        } while(false);
    } while(false);

    chdir(curDir);
    free(curDir);
    return result;
}

template <class ResourceHolder,  const char *treeLeafPath>
bool BaseObjectLoader<ResourceHolder, treeLeafPath>::serialize(const std::string &resourceName)
{
    ResourcesMapIt it = loadedObjectResources.find(resourceName);
    if(it == loadedObjectResources.end())
    {
        LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Cannot serialize resource, not found: ") << resourceName);
        return false;
    }
    return serialize(resourceName, it->second);
}


//Load Resources
template <class ResourceHolder, const char *treeLeafPath>
bool BaseObjectLoader<ResourceHolder, treeLeafPath>::loadResources()
{
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Loading resources starting: ") << getResourceTypeString(ResourceHolder::getResourceType()));
    //free resources
    //freeResources();

    //get directory
    char *curDir = get_current_dir_name();
    chdir(getResourceTreePath());

    DIR *objDir = opendir("./");
    if(!objDir)
    {
        LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Cannot open directory:") << getResourceTreePath() <<
                               LOG4CPLUS_TEXT(". Error: ") << strerror(errno));
        return false;
    }
    //iterate over directory and load resoures
    struct dirent *entry = NULL;
    while((entry = readdir(objDir)) != NULL)
    {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
        {
            //skip it
            continue;
        }
        //make files filter

        std::string filePath(entry->d_name);
        LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Load resource from file: ") << filePath);
        ResourcesMap res;
        try
        {
            res = ResourceClassType::loadResourcesImpl(filePath);
            if(res.empty())
            {
                LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("No resources loaded"));
                continue;
            }
            LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Loading resources finished: ") << res.size());

            if(ResourceClassType::isSerializable())
            {
                LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Serialize loaded resources"));
                std::for_each(res.begin(), res.end(), [this] (auto &value)
                {
                    this->serialize(value.first, value.second);
                });
            }
        }
        catch( const urc::SystemError &exception)
        {
            LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Exception error: ") << exception.m_errorCode <<
                                    LOG4CPLUS_TEXT(" description: ") << exception.m_description);
            continue;
        }
        //add to map
        loadedObjectResources.insert(res.begin(), res.end());
    }

    //close dir
    closedir(objDir);

    chdir(curDir);
    free(curDir);
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Total loaded resources:") << loadedObjectResources.size());
    return !loadedObjectResources.empty();
}


/////////////////////////DUMMY_PATH SPECIALIZATION///////////////////////////////////////////////////////

template <class ResourceHolder>
BaseObjectLoader<ResourceHolder, system_info_dummy_path>::BaseObjectLoader()
{
    logger = log4cplus::Logger::getInstance(getResourceTypeString(BaseObjectLoader<ResourceHolder, system_info_dummy_path>::getResourceType()));
}

template <class ResourceHolder>
typename BaseObjectLoader<ResourceHolder, system_info_dummy_path>::ResourceClassTypeCPtr BaseObjectLoader<ResourceHolder, system_info_dummy_path>::getResourceByName(const std::string &name) const
{
    ResourcesMapCIt it = loadedObjectResources.find(name);
    if(it != loadedObjectResources.end())
    {
        LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Get resource: ") << name);
        return it->second.get();    //dereferenced shared ptr
    }
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Cannot get resource: ") << name);
    return NULL;
}

template <class ResourceHolder>
bool BaseObjectLoader<ResourceHolder, system_info_dummy_path>::setResourceByName(
        const typename BaseObjectLoader<ResourceHolder, system_info_dummy_path>::ResourcesMap::key_type &name,
        const typename BaseObjectLoader<ResourceHolder, system_info_dummy_path>::ResourceClassTypeSharedPtr &resource)
{
    ResourcesMapIt it = loadedObjectResources.find(name);
    if(it == loadedObjectResources.end())
    {
        loadedObjectResources.insert(std::make_pair(name, resource));
        LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Insert resource: ") << name);
        return true;
    }
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Insert resource already exist: ") << name);
    return false;
}

template <class ResourceHolder>
void BaseObjectLoader<ResourceHolder, system_info_dummy_path>::freeResources()
{
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Free resources"));
    loadedObjectResources.clear();
}

template <class ResourceHolder>
bool BaseObjectLoader<ResourceHolder, system_info_dummy_path>::serialize(
    const std::string &resourceName,
    ResourceClassTypeSharedPtr &resource)
{
    /*char *curDir = get_current_dir_name();
    chdir(getResourceTreePath());


    chdir(curDir);
    free(curDir);*/
    return true;
}

template <class ResourceHolder>
bool BaseObjectLoader<ResourceHolder, system_info_dummy_path>::serialize(const std::string &resourceName)
{
    return true;
}

template <class ResourceHolder>
bool BaseObjectLoader<ResourceHolder, system_info_dummy_path>::deserialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource)
{
    return true;
}

template <class ResourceHolder>
bool BaseObjectLoader<ResourceHolder, system_info_dummy_path>::deserialize(const std::string &resourceName)
{
    return true;
}

template <class ResourceHolder>
bool BaseObjectLoader<ResourceHolder, system_info_dummy_path>::loadResources()
{
    //just resource doesn't need in path
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Loading resources starting: ") << getResourceTypeString(ResourceHolder::getResourceType()));
    ResourcesMap res;
    try
    {
        res = ResourceClassType::loadResourcesImpl();
        loadedObjectResources.insert(res.begin(), res.end());
    }
    catch( const urc::SystemError &exception)
    {
        LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Exception error: ") << exception.m_errorCode <<
                                LOG4CPLUS_TEXT(" description: ") << exception.m_description);
        return false;
    }
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Loading resources finished: ") << res.size());
    return !res.empty();
}
}
