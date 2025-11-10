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
    using ResourcesTuple = std::tuple<Loaders...>;
    using ResourceLoadersTuple = std::tuple<BaseObjectLoader<Loaders>...>;

    template<class Resource>
    using ResourceWeakPtr = std::weak_ptr<Resource>;

    template<class Resource>
    using ResourceConstWeakPtr = std::weak_ptr<const Resource>;

    LoadedResourcesHolder(const std::string &assetsPath, const std::string &tmpOperationsPath);
    ~LoadedResourcesHolder();

    //load resources for all ResourceLoaders
    template <class UsedTracer = Tracer<EmptyTracerImpl>>
    bool initResourceLoader(UsedTracer tracer = UsedTracer());

    //free resources for all ResourceLoaders
    bool deinitResourceLoader();

    //function to get specific resource from specific resource loader
    template <class Resource>
    ResourceConstWeakPtr<Resource> getResourcePtr(std::string_view name, bool needDeserialize = false);

    template <class Resource>
    ResourceWeakPtr<Resource> getResourceInstancePtr(std::string_view name, bool needDeserialize = false);

    //function to set specific resource to specific resource loader
    template <class Resource>
    bool insertResource(std::string_view name, std::shared_ptr<Resource> &&resourcePtr);

    //function to de/serialize object into specific file
    template<class Resource>
    bool serializeResource(std::string_view name);
    template<class Resource>
    bool deserializeResource(std::string_view name);

    const std::string &getAssetsPath() const;
    const std::string &getSerializationPath() const;
private:
    ResourceLoadersTuple loadersTuple;
    std::string m_assetsPath;
    std::string m_assetsTmpPath;
};
} /* namespace Resources */

#endif /* RESOURCESHOLDER_H_ */
