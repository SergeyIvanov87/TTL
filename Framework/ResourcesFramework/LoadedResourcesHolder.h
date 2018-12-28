#ifndef RESOURCESHOLDER_H_
#define RESOURCESHOLDER_H_

#include <tuple>
#include <array>
#include <utility>
#include <unistd.h>
#include <iostream>
#include <error.h>
#include <string.h>
#include <memory>
#include "BaseObjectLoader.h"

namespace Resources
{

//instantiated by Resource Loader template classes
template <class ... Loaders>
class LoadedResourcesHolder {
public:
    typedef std::tuple<Loaders...> ResourcesTuple;
    typedef std::tuple<BaseObjectLoader<Loaders>...> ResourceLoadersTuple;
    LoadedResourcesHolder(const std::string &assetsPath, const std::string &tmpOperationsPath);
    ~LoadedResourcesHolder();

    //function to get specific resource from specific resource loader
    template <class Resource>
    const Resource* getResourcePtr(const std::string &name, bool needDeserialize = false);

    //function to set specific resource to specific resource loader
    template <class Resource>
    bool insertResource(const std::string &name, std::shared_ptr<Resource> &&resourcePtr);

    //function to de/serialize object into specific file
    template<class Resource>
    bool serializeResource(const std::string &name);
    template<class Resource>
    bool deserializeResource(const std::string &name);

    const std::string &getAssetsPath() const;
    const std::string &getSerializationPath() const;
private:
    ResourceLoadersTuple loadersTuple;
    std::string m_assetsPath;
    std::string m_assetsTmpPath;
public:
    //load resources for all ResourceLoaders
    bool initResourceLoader();
    //free resources for all ResourceLoaders
    bool deinitResourceLoader();
};
} /* namespace Resources */

#endif /* RESOURCESHOLDER_H_ */
