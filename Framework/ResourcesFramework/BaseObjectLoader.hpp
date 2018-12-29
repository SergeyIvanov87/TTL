#ifndef BASE_OBJECT_LOADER_HPP
#define BASE_OBJECT_LOADER_HPP

#include "BaseObjectLoader.h"
namespace Resources
{
#define T_ARG_DEC              class ResourceHolder
#define T_ARG_DEF              ResourceHolder
static const char *tmpDirectory = "dumps";


template <T_ARG_DEC>
BaseObjectLoader<T_ARG_DEF>::BaseObjectLoader()
{
    logger = log4cplus::Logger::getInstance(BaseObjectLoader<T_ARG_DEF>::getResourceTypeDescription());
}

//Get/Set
template <T_ARG_DEC>
typename BaseObjectLoader<T_ARG_DEF>::ResourceClassTypeCPtr BaseObjectLoader<T_ARG_DEF>::getResourceByName(const std::string &name) const
{
    auto it = loadedObjectResources.find(name);
    if(it != loadedObjectResources.end())
    {
        LOG4CPLUS_TRACE(logger, LOG4CPLUS_TEXT("Get resource: ") << name <<
                                LOG4CPLUS_TEXT(" Type: ") << ResourceHolder::getResourceTypeDescription());
        return it->second.get();    //dereferenced shared ptr
    }
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Cannot get resource: ") << name <<
                            LOG4CPLUS_TEXT(" Type: ") << ResourceHolder::getResourceTypeDescription());
    return nullptr;
}

template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::setResourceByName(
        const typename BaseObjectLoader<T_ARG_DEF>::ResourcesMap::key_type &name,
        const typename BaseObjectLoader<T_ARG_DEF>::ResourceClassTypeSharedPtr &resource)
{
    auto it = loadedObjectResources.find(name);
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
template <T_ARG_DEC>
void BaseObjectLoader<T_ARG_DEF>::freeResources()
{
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Free resources"));
    loadedObjectResources.clear();
}

//Deserialize/Serialize
template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::deserialize(const std::string &resourceName)
{
    if constexpr(ResourcesTraits<T_ARG_DEF>::hasAssetsPath())
    {
        auto it = loadedObjectResources.find(resourceName);
        if(it == loadedObjectResources.end())
        {
            LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Cannot deserialize resource, not found: ") << resourceName);
            return false;
        }
        return doDeserialize(resourceName, it->second);
    }
    return true;
}

template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::serialize(const std::string &resourceName)
{
    if constexpr(ResourcesTraits<T_ARG_DEF>::hasAssetsPath())
    {
        auto it = loadedObjectResources.find(resourceName);
        if(it == loadedObjectResources.end())
        {
            LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Cannot serialize resource, not found: ") << resourceName);
            return false;
        }
        return doSerialize(resourceName, it->second);
    }
    return true;
}

//Load Resources
template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::loadResources()
{
    if constexpr (!ResourcesTraits<T_ARG_DEF>::hasAssetsPath())
    {
        return doLoadResourcesFromFS();
    }
    return doLoadResourcesFromMemory();
}


template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::doLoadResourcesFromFS()
{
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Loading resources from FS starting: ") << ResourceHolder::getResourceTypeDescription());
    //free resources
    //freeResources();

    //get directory
    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    chdir(ResourcesTraits<T_ARG_DEF>::getResourcePath());

    DIR *objDir = opendir("./");
    if(!objDir)
    {
        LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Cannot open directory:") << ResourcesTraits<T_ARG_DEF>::getResourcePath() <<
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
        //TODO make files filter

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
                    this->doSerialize(value.first, value.second);
                });
            }
        }
        catch( const urc::SystemError &exception)
        {
            LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Exception error: ") << exception.to_string());
            continue;
        }

        //move to map
        loadedObjectResources.merge(std::move(res));
    }

    //close dir
    closedir(objDir);
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Total loaded resources:") << loadedObjectResources.size());
    return !loadedObjectResources.empty();
    }
}

template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::doLoadResourcesFromMemory()
{
    //just resource doesn't need in path
    size_t initialSize = loadedObjectResources.size();
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Loading resources starting: ") << ResourceHolder::getResourceTypeDescription());
    try
    {
        loadedObjectResources.merge(ResourceClassType::loadResourcesImpl());
    }
    catch( const urc::SystemError &exception)
    {
        LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Exception error: ") << exception.to_string());
        return false;
    }
    size_t diff = loadedObjectResources.size() - initialSize;
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Loading resources finished: ")
                           << diff <<  LOG4CPLUS_TEXT(", total: ") << loadedObjectResources.size() );
    return diff;
}

template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::doDeserialize(const std::string &resourceName, ResourceClassTypeSharedPtr &resource)
{
    if((!resource))
    {
        LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Empty resource pointer for name: ") << resourceName);
        return false;
    }

    if(!resource->wasSerialized())
    {
        LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("resource was not serialized. skip it: ") << resourceName);
        return true;
    }

    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Current directory: ") << curDirPtr.get());
    chdir(ResourcesTraits<T_ARG_DEF>::getResourcePath());
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Change directory: ") << ResourcesTraits<T_ARG_DEF>::getResourcePath());

    //change to tmpDirectory
    if(-1 == chdir(tmpDirectory))
    {
        LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Cannot change serialize directory: ") <<
                                curDirPtr.get() << LOG4CPLUS_TEXT("/") <<
                                ResourcesTraits<T_ARG_DEF>::getResourcePath() << LOG4CPLUS_TEXT("/") <<
                                tmpDirectory);
        throw urc::FileOpenError(std::string(curDirPtr.get()) + ResourcesTraits<T_ARG_DEF>::getResourcePath() + tmpDirectory, errno);
    }

    //open file for reading
    std::string fileName = resourceName + ".dump";
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Open file name: ") <<
                           fileName <<
                           LOG4CPLUS_TEXT(" to deserialize resource"));

    std::ifstream fileIn(fileName, std::ios::in | std::ios::binary);
    if(!fileIn.is_open())
    {
        LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Cannot open file for reading: ") << strerror(errno));
        throw urc::FileOpenError(fileName, errno);
    }
    bool result = resource->deserialize(fileIn);
    fileIn.close();
    return result;
}

template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::doSerialize(
    const std::string &resourceName,
    ResourceClassTypeSharedPtr &resource)
{
    bool result = false;
    if((!resource))
    {
        LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Empty resource pointer for name: ") << resourceName);
        return result;
    }

/* TODO - BC or not BC? */
    if(!resource->wasSerialized())
    {
        LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("resource was not serialized. skip it: ") << resourceName);
        return true;
    }

    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Current directory: ") << curDirPtr.get());
    chdir(ResourcesTraits<T_ARG_DEF>::getResourcePath());
    LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("Change directory: ") << ResourcesTraits<T_ARG_DEF>::getResourcePath());

    //change to tmpDirectory
    if(-1 == chdir(tmpDirectory))
    {
        if((errno != ENOENT) || (-1 == mkdir(tmpDirectory, S_IRWXO | S_IRWXG | S_IRWXU | S_IFDIR)))
        {
            LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Cannot create serialize directory: ") <<
                                    curDirPtr.get() << LOG4CPLUS_TEXT("/") <<
                                    ResourcesTraits<T_ARG_DEF>::getResourcePath() << LOG4CPLUS_TEXT("/") <<
                                    tmpDirectory);

                throw urc::FileOpenError(std::string(curDirPtr.get()) + ResourcesTraits<T_ARG_DEF>::getResourcePath() + tmpDirectory, errno);
        }
    }

    //open file for dump
    std::string fileName = resourceName + ".dump";
    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Open file name: ") <<
                           fileName <<
                           LOG4CPLUS_TEXT(" to dump resource"));

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
    return result;
}
#undef T_ARG_DEC
#undef T_ARG_DEF
}

#endif //BASE_OBJECT_LOADER_HPP
