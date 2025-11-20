#ifndef BASE_OBJECT_LOADER_HPP
#define BASE_OBJECT_LOADER_HPP
#include <regex>
#include "BaseObjectLoader.h"
#include <ttl/include/Utils/StringUtils.h>

namespace Resources
{
#define T_ARG_DEC              class ResourceHolder
#define T_ARG_DEF              ResourceHolder
static const char *tmpDirectory = "dumps";

//Get/Set
template <T_ARG_DEC>
typename BaseObjectLoader<T_ARG_DEF>::OwningPtrConst
BaseObjectLoader<T_ARG_DEF>::getResourceByName(std::string_view name) const
{
    if(auto it = loadedObjectResources.find(name.data()); it != loadedObjectResources.end())
    {
        return it->second;
    }
    throw urc::MissingResourceError(name, makeString("Doesn't exist in a collections of \"", getResourceTypeDescription(), "\" objects, size: ", loadedObjectResources.size()));
}

template <T_ARG_DEC>
typename BaseObjectLoader<T_ARG_DEF>::OwningPtr
BaseObjectLoader<T_ARG_DEF>::getResourceByName(std::string_view name)
{
    return std::const_pointer_cast<ResourceClassType>(static_cast<const Self*>(this)->getResourceByName(name));
}


template <T_ARG_DEC>
typename BaseObjectLoader<T_ARG_DEF>::NonOwningPtrConst
BaseObjectLoader<T_ARG_DEF>::getNonOwnResourceByName(std::string_view name) const
{
    return getResourceByName(name);
}

template <T_ARG_DEC>
typename BaseObjectLoader<T_ARG_DEF>::NonOwningPtr
BaseObjectLoader<T_ARG_DEF>::getNonOwnResourceByName(std::string_view name)
{
    return getResourceByName(name);
}

template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::setResourceByName(
        std::string_view name,
        const typename BaseObjectLoader<T_ARG_DEF>::OwningPtr &resource)
{
    auto it = loadedObjectResources.find(name.data());
    if(it == loadedObjectResources.end())
    {
        loadedObjectResources.insert(std::make_pair(name, resource));
        return true;
    }
    return false;
}

template <T_ARG_DEC>
size_t BaseObjectLoader<T_ARG_DEF>::size() const {
    return loadedObjectResources.size();
}

//Free
template <T_ARG_DEC>
void BaseObjectLoader<T_ARG_DEF>::freeResources()
{
    loadedObjectResources.clear();
}

//Deserialize/Serialize
template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::deserialize(std::string_view resourceName)
{
    if constexpr (ResourcesTraits<T_ARG_DEF>::hasAssetsPath())
    {
        auto it = loadedObjectResources.find(resourceName.data());
        if(it == loadedObjectResources.end())
        {
            return false;
        }
        return doDeserialize(resourceName, it->second);
    }
    return true;
}

template <T_ARG_DEC>
bool BaseObjectLoader<T_ARG_DEF>::serialize(std::string_view resourceName)
{
    if constexpr (ResourcesTraits<T_ARG_DEF>::hasAssetsPath())
    {
        auto it = loadedObjectResources.find(resourceName.data());
        if(it == loadedObjectResources.end())
        {
            return false;
        }
        return doSerialize(resourceName, it->second);
    }
    return true;
}

//Load Resources
template <T_ARG_DEC>
template<class UsedTracer>
size_t BaseObjectLoader<T_ARG_DEF>::loadResources(UsedTracer tracer)
{
    if constexpr (ResourcesTraits<T_ARG_DEF>::hasAssetsPath())
    {
        return doLoadResourcesFromFS(tracer);
    }
    else
    {
        return doLoadResourcesFromMemory(tracer);
    }
}


template <T_ARG_DEC>
template<class UsedTracer>
size_t BaseObjectLoader<T_ARG_DEF>::doLoadResourcesFromFS(UsedTracer &tracer)
{
    size_t initialSize = loadedObjectResources.size();

    //get directory
    tracer.trace("Current Directory: ", get_current_dir_name());
    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    tracer.trace("Go into Directory: ", ResourcesTraits<T_ARG_DEF>::getResourcePath());
    if(-1 == chdir(ResourcesTraits<T_ARG_DEF>::getResourcePath()))
    {
        throw urc::FileOpenError(ResourcesTraits<T_ARG_DEF>::getResourcePath(), errno);
    }

    DIR *objDir = opendir("./");
    if(!objDir)
    {
        throw urc::FileOpenError(ResourcesTraits<T_ARG_DEF>::getResourcePath(), errno);
    }

    tracer.trace("Loading \"", ResourceHolder::getResourceTypeDescription(), "\" STARTED. Initial count: ", loadedObjectResources.size());
    tracer.trace("Use regex: ", ResourcesTraits<T_ARG_DEF>::getResourceRegexFilter());
    const std::basic_regex ext_regex(ResourcesTraits<T_ARG_DEF>::getResourceRegexFilter());

    //iterate over directory and load resoures
    struct dirent *entry = NULL;
    size_t totalFilesChecked = 0;
    while((entry = readdir(objDir)) != NULL)
    {
        auto tracer4File = tracer;
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
        {
            //skip it
            continue;
        }

        totalFilesChecked++;

        std::string filePath(entry->d_name);
        if(!std::regex_match(filePath, ext_regex))
        {
            tracer4File.trace("Source: ", filePath, " <SKIPPED>");
            continue;
        }

        tracer4File.trace("Source: ", filePath);
        ResourcesMap res;
        try
        {
            res = ResourceClassType::loadResources(filePath, tracer);
            if(res.empty())
            {
                tracer4File << "\tNo resources loaded";
                continue;
            }
            tracer4File.trace("\tLoaded count: ", res.size());

            if constexpr (ResourceClassType::isSerializable())
            {
                std::for_each(res.begin(), res.end(), [this, &tracer4File] (auto &value)
                {
                    this->doSerialize(value.first, value.second, tracer4File);
                });
            }
        }
        catch( const urc::SystemError &exception)
        {
            tracer4File.trace("\tEXCEPTION: ", exception.to_string());
            closedir(objDir);
            throw;
        }

        //move to map
        loadedObjectResources.merge(std::move(res));
    }

    //close dir
    closedir(objDir);
    size_t diff = loadedObjectResources.size() - initialSize;
    tracer.trace("Loading \"", ResourceHolder::getResourceTypeDescription(), "\" FINISHED. Loaded count: ", diff, ", total files in directory:", totalFilesChecked, "\n");
    return diff;
}

template <T_ARG_DEC>
template<class UsedTracer>
size_t BaseObjectLoader<T_ARG_DEF>::doLoadResourcesFromMemory(UsedTracer &tracer)
{
    //just resource doesn't need in path
    size_t initialSize = loadedObjectResources.size();
    tracer.trace("Loading \"", ResourceHolder::getResourceTypeDescription(), "\" STARTED. Initial count: ");

    try
    {
        loadedObjectResources.merge(ResourceClassType::loadResources("", tracer));
    }
    catch( const urc::SystemError &exception)
    {
        tracer.trace("\tEXCEPTION: ", exception.to_string());
        throw;
    }

    size_t diff = loadedObjectResources.size() - initialSize;
    tracer.trace("Loading \"", ResourceHolder::getResourceTypeDescription(), "\" FINISHED. Total count: ", diff, "\n");
    return diff;
}

template <T_ARG_DEC>
template <class UsedTracer>
bool BaseObjectLoader<T_ARG_DEF>::doDeserialize(
                                                std::string_view resourceName,
                                                OwningPtr &resource,
                                                UsedTracer tracer)
{
    tracer.trace("Deserialize for \"", resourceName, "\":");
    if((!resource))
    {
        return false;
    }

    if(!resource->wasSerialized())
    {
        tracer << "Do not need to deserialize\n";
        return true;
    }

    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    chdir(ResourcesTraits<T_ARG_DEF>::getResourcePath());

    //change to tmpDirectory
    if(-1 == chdir(tmpDirectory))
    {
        urc::FileOpenError ex(std::string(curDirPtr.get()) + "/" + ResourcesTraits<T_ARG_DEF>::getResourcePath() + "\"" + tmpDirectory, errno);
        tracer << ex.what();
        throw ex;
    }

    //open file for reading
    std::string fileName(resourceName);
    fileName += ".dump";
    std::ifstream fileIn(fileName, std::ios::in | std::ios::binary);
    if(!fileIn.is_open())
    {
        urc::FileOpenError ex(fileName, errno);
        tracer << ex.what();
        throw ex;
    }

    size_t si = resource->getObjectSize();
    bool result = resource->deserialize(fileIn);
    fileIn.close();
    size_t si_after = resource->getObjectSize();
    tracer.trace("To file: ", fileName, ", status: ", result, ". Object size before: ", si, " after: ", si_after);
    return result;
}

template <T_ARG_DEC>
template <class UsedTracer>
bool BaseObjectLoader<T_ARG_DEF>::doSerialize(
                                                std::string_view resourceName,
                                                OwningPtr &resource,
                                                UsedTracer tracer)
{
    tracer.trace("Serialize for \"", resourceName, "\":");
    bool result = false;
    if((!resource))
    {
        return result;
    }

/* TODO - BC or not BC? */
    if(resource->wasSerialized())
    {
        tracer << "Do not need to serialize\n";
        return true;
    }

    std::unique_ptr<char, std::function<void(char *)>> curDirPtr(get_current_dir_name(), [](char *ptr) -> void
    {
        chdir(ptr);
        free(ptr);
    });

    chdir(ResourcesTraits<T_ARG_DEF>::getResourcePath());

    //change to tmpDirectory
    while(-1 == chdir(tmpDirectory))
    {
        if(errno != ENOENT)
        {
            urc::FileOpenError ex(std::string("Cannot change dir to: ") + curDirPtr.get() + "/" + ResourcesTraits<T_ARG_DEF>::getResourcePath() + "/" + tmpDirectory, errno);
            tracer << ex.what();
            throw ex;
        }

        if(-1 == mkdir(tmpDirectory, S_IRWXO | S_IRWXG | S_IRWXU | S_IFDIR))
        {
            urc::FileOpenError ex(std::string(curDirPtr.get()) + "/" + ResourcesTraits<T_ARG_DEF>::getResourcePath() + "/" + tmpDirectory, errno);
            tracer << ex.what();
            throw ex;
        }
    }

    //open file for dump
    std::string fileName(resourceName);
     fileName += ".dump";
    std::ofstream fileOut(fileName, std::ios::out | std::ios::binary);
    if(!fileOut.is_open())
    {
        urc::FileOpenError ex(fileName, errno);
        tracer << ex.what();
        throw ex;
    }
    size_t si = resource->getObjectSize();
    result = resource->serialize(fileOut);
    size_t si_after = resource->getObjectSize();
    tracer.trace("Object size before: ", si, " after: ", si_after);
    fileOut.close();
    return result;
}
#undef T_ARG_DEC
#undef T_ARG_DEF
}

#endif //BASE_OBJECT_LOADER_HPP
