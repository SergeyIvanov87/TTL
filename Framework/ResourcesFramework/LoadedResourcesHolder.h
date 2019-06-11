#ifndef RESOURCESHOLDER_H_
#define RESOURCESHOLDER_H_

#include <tuple>
#include <array>
#include <utility>
#include <unistd.h>
#include <iostream>
#include <error.h>
#include <string.h>
#include <string_view>
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
    const Resource* getResourcePtr(std::string_view name, bool needDeserialize = false);

    //function to set specific resource to specific resource loader
    template <class Resource>
    bool insertResource(std::string_view name, std::shared_ptr<Resource> &&resourcePtr);

    //function to de/serialize object into specific file
    template<class Resource>
    bool serializeResource(std::string_view name);
    template<class Resource>
    bool deserializeResource(std::string_view name);

    const std::string &getAssetsPath() const;
    const std::string &getSerializationPath() const {  return m_assetsTmpPath; }
private:
    ResourceLoadersTuple loadersTuple;
    std::string m_assetsPath;
    std::string m_assetsTmpPath;
public:
    //load resources for all ResourceLoaders
    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    bool initResourceLoader(UsedTracer tracer = UsedTracer());
    //free resources for all ResourceLoaders
    bool deinitResourceLoader();
};
} /* namespace Resources */

#endif /* RESOURCESHOLDER_H_ */
